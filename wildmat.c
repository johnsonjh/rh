/*
** wildmat.c
**  $Revision: 1.2 $
**
**  Do shell-style pattern matching for ?, \, [], and * characters.
**  Might not be robust in face of malformed patterns; e.g., "foo[a-"
**  could cause a segmentation violation.  It is 8bit clean.
**
**  Written by Rich $alz, mirror!rs, Wed Nov 26 19:03:17 EST 1986.
**  Rich $alz is now <rsalz@osf.org>.
**  April, 1991:  Replaced mutually-recursive calls with in-line code
**  for the star character.
**
**  Special thanks to Lars Mathiesen <thorinn@diku.dk> for the ABORT code.
**  This can greatly speed up failing wildcard patterns.  For example:
**	pattern: -*-*-*-*-*-*-12-*-*-*-m-*-*-*
**	text 1:	 -adobe-courier-bold-o-normal--12-120-75-75-m-70-iso8859-1
**	text 2:	 -adobe-courier-bold-o-normal--12-120-75-75-X-70-iso8859-1
**  Text 1 matches with 51 calls, while text 2 fails with 54 calls.  Without
**  the ABORT code, it takes 22310 calls to fail.  Ugh.  The following
**  explanation is from Lars:
**  The precondition that must be fulfilled is that DoMatch will consume
**  at least one character in text.  This is true if *p is neither '*' nor
**  '\0'.)  The last return has ABORT instead of FALSE to avoid quadratic
**  behaviour in cases like pattern "*a*b*c*d" with text "abcxxxxx".  With
**  FALSE, each star-loop has to run to the end of the text; with ABORT
**  only the last one does.
**
**  Once the control of one instance of DoMatch enters the star-loop, that
**  instance will return either TRUE or ABORT, and any calling instance
**  will therefore return immediately after (without calling recursively
**  again).  In effect, only one star-loop is ever active.  It would be
**  possible to modify the code to maintain this context explicitly,
**  eliminating all recursive calls at the cost of some complication and
**  loss of clarity (and the ABORT stuff seems to be unclear enough by
**  itself).  I think it would be unwise to try to get this into a
**  released version unless you have a good test data base to try it out
**  on.
*/

#if !defined(lint)
static char rcsid[] = "$Id: wildmat.c,v 1.2 2011/12/31 02:41:07 vandys Exp $";
#endif

#define TRUE			1
#define FALSE			0
#define ABORT			-1


    /* What character marks an inverted character class? */
#define NEGATE_CLASS		'^'
    /* Is "*" a common pattern? */
#define OPTIMIZE_JUST_STAR
    /* Do tar(1) matching rules, which ignore a trailing slash? */
#undef MATCH_TAR_PATTERN


static unsigned char charmap[] = {
    '\000', '\001', '\002', '\003', '\004', '\005', '\006', '\007',
    '\010', '\011', '\012', '\013', '\014', '\015', '\016', '\017',
    '\020', '\021', '\022', '\023', '\024', '\025', '\026', '\027',
    '\030', '\031', '\032', '\033', '\034', '\035', '\036', '\037',
    '\040', '\041', '\042', '\043', '\044', '\045', '\046', '\047',
    '\050', '\051', '\052', '\053', '\054', '\055', '\056', '\057',
    '\060', '\061', '\062', '\063', '\064', '\065', '\066', '\067',
    '\070', '\071', '\072', '\073', '\074', '\075', '\076', '\077',
    '\100', '\141', '\142', '\143', '\144', '\145', '\146', '\147',
    '\150', '\151', '\152', '\153', '\154', '\155', '\156', '\157',
    '\160', '\161', '\162', '\163', '\164', '\165', '\166', '\167',
    '\170', '\171', '\172', '\133', '\134', '\135', '\136', '\137',
    '\140', '\141', '\142', '\143', '\144', '\145', '\146', '\147',
    '\150', '\151', '\152', '\153', '\154', '\155', '\156', '\157',
    '\160', '\161', '\162', '\163', '\164', '\165', '\166', '\167',
    '\170', '\171', '\172', '\173', '\174', '\175', '\176', '\177',
    '\200', '\201', '\202', '\203', '\204', '\205', '\206', '\207',
    '\210', '\211', '\212', '\213', '\214', '\215', '\216', '\217',
    '\220', '\221', '\222', '\223', '\224', '\225', '\226', '\227',
    '\230', '\231', '\232', '\233', '\234', '\235', '\236', '\237',
    '\240', '\241', '\242', '\243', '\244', '\245', '\246', '\247',
    '\250', '\251', '\252', '\253', '\254', '\255', '\256', '\257',
    '\260', '\261', '\262', '\263', '\264', '\265', '\266', '\267',
    '\270', '\271', '\272', '\273', '\274', '\275', '\276', '\277',
    '\300', '\341', '\342', '\343', '\344', '\345', '\346', '\347',
    '\350', '\351', '\352', '\353', '\354', '\355', '\356', '\357',
    '\360', '\361', '\362', '\363', '\364', '\365', '\366', '\367',
    '\370', '\371', '\372', '\333', '\334', '\335', '\336', '\337',
    '\340', '\341', '\342', '\343', '\344', '\345', '\346', '\347',
    '\350', '\351', '\352', '\353', '\354', '\355', '\356', '\357',
    '\360', '\361', '\362', '\363', '\364', '\365', '\366', '\367',
    '\370', '\371', '\372', '\373', '\374', '\375', '\376', '\377',
};


/*
**  Match text and p, return TRUE, FALSE, or ABORT.
*/
static int
DoMatch(unsigned char	*text,
	unsigned char	*pattern,
	int			 igncase)
{
    int		 last;
    int 	 	 matched;
    int 	 	 reverse;
    unsigned char	*tp;
    unsigned char	*class_end;
    unsigned char	*cmap = charmap;
    
    for ( ; *pattern != '\0'; text++, pattern++) {
	
	if (*text == '\0') {
	    return (*pattern == '*' && *++pattern == '\0') ? TRUE : ABORT;
	}

	switch (*pattern) {
	    
	case '\\':
	    /* Literal match with following character. */
	    if (*(pattern + 1)) {
		pattern++;
	    }
	    /* FALLTHROUGH */
	default:
	    if (igncase) {
		if (cmap[*text] != cmap[*pattern]) {
		    return FALSE;
		}
	    }
	    else if (*text != *pattern) {
		return FALSE;
	    }
	    continue;
	    
	case '?':
	    /* Match anything. */
	    continue;
	    
	case '*':
	    while (*++pattern == '*') {
		/* Consecutive stars act just like one. */
		continue;
	    }
	    if (*pattern == '\0') {
		/* Trailing star matches everything. */
		return TRUE;
	    }
	    while (*text) {
		if ((matched = DoMatch(text++, pattern, igncase)) != FALSE) {
		    return matched;
		}
	    }
	    return ABORT;
	    
	case '[':
	    for (tp = ++pattern; *tp != '\0' && *tp != ']'; tp++) {
		if (*tp == '\\') {
		    if (*++tp == '\0') {
			break;
		    }
		}
	    }
	    if (*tp == '\0' || tp == pattern) {
		/* Invalid character class so just do match against '[' */
		if (*text != '[') {
		    return FALSE;
		}
		pattern--;
		continue;
	    }
	    class_end = tp;
	    
	    /* inverse character class. */
	    if (*pattern == NEGATE_CLASS && *(pattern + 1) != ']') {
		reverse = TRUE;
		pattern++;
	    }
	    else {
		reverse = FALSE;
	    }
	    
	    for (tp = pattern, last = *pattern, matched = FALSE;
		 *pattern != '\0' && *pattern != ']';
		 last = *pattern, pattern++) {
		if (*pattern == '-'
		    && pattern != tp
		    && *(pattern + 1) != ']') {
		    if (*++pattern == '\\') {
			pattern++;
		    }
		    if (igncase) {
			if (cmap[*text] <= cmap[*pattern]
			    && cmap[*text] >= cmap[last]) {
			    pattern = class_end;
			    matched = TRUE;
			    break;
			}
		    }
		    else if (*text <= *pattern && *text >= last) {
			pattern = class_end;
			matched = TRUE;
			break;
		    }
		}
		else {
		    if (*pattern == '\\') {
			pattern++;
		    }
		    if (igncase) {
			if (cmap[*text] == cmap[*pattern]) {
			    pattern = class_end;
			    matched = TRUE;
			    break;
			}
		    }
		    else if (*text == *pattern) {
			pattern = class_end;
			matched = TRUE;
			break;
		    }
		}
	    }
	    
	    if (matched == reverse) {
		return FALSE;
	    }
	    continue;
	}
    }
    
#if defined(MATCH_TAR_PATTERN)
    if (*text == '/') {
	return TRUE;
    }
#endif	/* MATCH_TAR_PATTERN */
    return *text == '\0';
}


/*
**  User-level routine.  Returns TRUE or FALSE.
*/
int
wildmat(unsigned char	*text,
	unsigned char	*pattern,
	int		 dotspec,
	int		 igncase)
{
#if defined(OPTIMIZE_JUST_STAR)
    if (pattern[0] == '*' && pattern[1] == '\0') {
	return (dotspec && *text == '.') ? FALSE : TRUE;
    }
#endif	/* OPTIMIZE_JUST_STAR */
    if (dotspec && *text == '.' && *pattern != '.') {
	return FALSE;
    }
    else {
	return DoMatch(text, pattern, igncase) == TRUE;
    }
}



#if defined(TEST)

#include <stdio.h>

int
main(int	 argc,
     char	*argv[])
{
    int		 dotspec = 0;
    int		 igncase = 0;
    char	 p[80];
    char	 text[80];

    (void) printf("Wildmat tester.\n"
		  "Enter pattern, then strings to test.\n"
		  "A blank line gets prompts for a new pattern\n"
		  "A blank pattern exits the program.\n"
		  );

    for ( ; ; ) {
	(void) printf("\nEnter pattern:  ");
	(void) fflush(stdout);
	if (fgets(p, sizeof(p), stdin) == NULL || p[0] == '\n') {
	    break;
	}
	if (p[strlen(p) - 1] == '\n') {
	    p[strlen(p) - 1] == '\0';
	}
	for ( ; ; ) {
	    (void) printf("Enter text:  ");
	    (void) fflush(stdout);
	    if (fgets(text, sizeof(text), stdin) == NULL) {
		exit(0);
	    }
	    if (text[0] == '\n') {
		/* Blank line; go back and get a new pattern. */
		break;
	    }
	    if (text[strlen(text) - 1] == '\n') {
		text[strlen(text) - 1] == '\0';
	    }
	    (void) printf("      %s\n",
			  wildmat(text, p, dotspec, igncase) ? "YES" : "NO");
	}
    }

    return 0;
}

#endif	/* defined(TEST) */

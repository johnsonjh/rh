/* -*-C-*- */

%{

/*
 * rhlex.l
 *
 * Written by:	Rick Ohnemus	(rick@sterling.com)
 */
#define _FILE_OFFSET_BITS 64
#include <memory.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <errno.h>

#if defined(DEBUG)
#  include <assert.h>
#endif

#include "rh.h"
#include "rhdata.h"
#include "rhgram.h"
#include "alloc.h"
#include "errmsg.h"
#include "sysinfo.h"

/* #define YY_DECL int rh_lex YY_PROTO(( void )) */
#define YY_DECL int rh_lex(void)

#define CHUNK_SIZE	32

#undef YY_INPUT
#define YY_INPUT(buf, result, max_size) (result = rh_read(buf, max_size))

#define RH_ATIME	1
#define RH_CTIME	2
#define RH_MTIME	3

static char *	Start_line = (char *) NULL;
static int	Start_line_num;
static int	Start_line_pos;
static int	First_line = 1;
char *		Line = (char *) NULL;
int		Line_len = 0;
int		Line_num = 0;
int		Line_pos = 0;
static int	OldSC;

static date_info		get_file_time(char *str);
static void			lex_error(const char * line,
					  int line_num,
					  const char *msg);
static int			lookup(const char *str);
extern void			rh_error();
static int			rh_read(char *buf, int max_size);
static void			remove_escapes(char *str);
extern long			strtol();

%}

%x COMMENT
%x DATE_SPEC
%x FILE_NAME
%x FILE_TIME
%x FIRST_LINE
%x NEW_LINE

%%

    if (First_line) {
	BEGIN(FIRST_LINE);
	First_line = 0;
    }

<FIRST_LINE>.*		{
			    Line = ALLOCATE(yyleng + 1);
			    (void) strcpy(Line, yytext);
			    Line_len = yyleng;
			    Line_num = 1;
			    Line_pos = 0;
			    BEGIN(INITIAL);
			    yyless(0);
			}

<FIRST_LINE>\n		{
			    Line = ALLOCATE(81);
			    *Line = '\0';
			    Line_len = 80;
			    Line_num = 1;
			    Line_pos = 0;
			    BEGIN(INITIAL);
			    yyless(0);
			}

<FIRST_LINE><<EOF>>	{ yyterminate(); }

<NEW_LINE>.*		{
			    int adjustment = 0;
			    int len;
			    int lp;
			    char *from;
			    char *to;

			    for (from = yytext; *from != '\0'; from++) {
				if (*from == '\t') {
				    adjustment += 8;
				}
			    }
			    len = yyleng + adjustment;
			    if (len > Line_len) {
				Line = REALLOCATE(Line, len + 1);
				Line_len = len;
			    }
			    if (adjustment) {
				for (from = yytext, to = Line, lp = 0;
				     *from != '\0';
				     /* void */) {
				    if (*from == '\t') {
					adjustment = 8 - (lp + 1) % 8;
					lp += adjustment;
					while (adjustment) {
					    *to++ = ' ';
					    adjustment--;
					}
					from++;
				    }
				    else {
					*to++ = *from++;
					lp++;
				    }
				}
				*to = *from;
			    }
			    else {
				(void) strcpy(Line, yytext);
			    }
			    Line_num++;
			    Line_pos = 0;
			    BEGIN(OldSC);
			    yyless(0);
			}

<NEW_LINE>\n		{ *Line = '\0'; Line_num++; Line_pos = 0; }

<NEW_LINE><<EOF>>	{
			    if (OldSC == COMMENT) {
				lex_error(Start_line, Start_line_num,
					  "unterminated comment");
			    }
			    yyterminate();
			}

<COMMENT>[^*\n]*	{ Line_pos += yyleng; }

<COMMENT>[^*\n]*\n	{ OldSC = COMMENT; BEGIN(NEW_LINE); }

<COMMENT>"*"+[^*/\n]*	{ Line_pos += yyleng; }

<COMMENT>"*"+[^*/\n]*\n	{ OldSC = COMMENT; BEGIN(NEW_LINE); }

<COMMENT>"*"+"/"	{
			    Line_pos += yyleng;
			    free(Start_line);
			    Start_line = (char *) NULL;
			    BEGIN(INITIAL);
			}

<COMMENT><<EOF>>	{
			    lex_error(Start_line, Start_line_num,
				      "unterminated comment");
			}

<DATE_SPEC>[^\\\]\n]*	{ yymore(); }

<DATE_SPEC>\\[^\n]	{ yymore(); }

<DATE_SPEC>\\		{ yymore(); }

<DATE_SPEC>\]		{
			    char *date_str;

			    if (yyleng == 1) {
				lex_error(Line, Line_num,
					  "empty date specification");
			    }
			    Line_pos += yyleng;
			    date_str = SAVESTR(yytext);
			    date_str[yyleng - 1] = '\0';
			    remove_escapes(date_str);
			    rh_lval.di = ALLOCATE(sizeof(date_info));
			    *rh_lval.di = getdate(date_str,
						  (struct timeb *) NULL);
			    free(date_str);
			    if (rh_lval.di->seconds == -1) {
				rh_error("invalid date specification");
			    }
			    BEGIN(INITIAL);
			    return DATESPEC;
			}

<DATE_SPEC>\n		{
			    lex_error(Line, Line_num,
				      "unterminated date specification");
			}

<DATE_SPEC><<EOF>>	{
			    lex_error(Line, Line_num,
				      "unterminated date specification");
			}

<FILE_NAME>[^\\\"\n]*	{ yymore(); }

<FILE_NAME>\\[^\n]	{ yymore(); }

<FILE_NAME>\\		{ yymore(); }

<FILE_NAME>\"		{
			    if (yyleng == 1) {
				lex_error(Line, Line_num,
					  "empty file name pattern");
			    }
			    Line_pos += yyleng;
			    rh_lval.str = SAVESTR(yytext);
			    rh_lval.str[yyleng - 1] = '\0';
			    BEGIN(INITIAL);
			    remove_escapes(rh_lval.str);
			    return FILENAME_PATTERN;
			}

<FILE_NAME>\n		{
			    lex_error(Line, Line_num,
				      "unterminated file name pattern");
			}

<FILE_NAME><<EOF>>	{
			    lex_error(Line, Line_num,
				      "unterminated file name pattern");
			}

<FILE_TIME>[^\\#\n]*	{ yymore(); }

<FILE_TIME>\\[^\n]	{ yymore(); }

<FILE_TIME>\\		{ yymore(); }

<FILE_TIME>#		{
			    char *ftspec;

			    if (yyleng == 1) {
				lex_error(Line, Line_num,
					  "empty file time operator");
			    }
			    ftspec = SAVESTR(yytext);
			    ftspec[yyleng - 1] = '\0';
			    rh_lval.di = ALLOCATE(sizeof(date_info));
			    *rh_lval.di = get_file_time(ftspec);
			    Line_pos += yyleng;
			    free(ftspec);
			    BEGIN(INITIAL);
			    return FILETIME_FIELD;
			}

<FILE_TIME>\n		{
			    lex_error(Line, Line_num,
				      "unterminated file time operator");
			}

<FILE_TIME><<EOF>>	{
			    lex_error(Line, Line_num,
				      "unterminated file time operator");
			}

"!"			{ Line_pos += yyleng; return '!'; }
"!="			{ Line_pos += yyleng; return NE; }
"%"			{ Line_pos += yyleng; return '%'; }
"&"			{ Line_pos += yyleng; return '&'; }
"&&"			{ Line_pos += yyleng; return LOGAND; }
"("			{ Line_pos += yyleng; return '('; }
")"			{ Line_pos += yyleng; return ')'; }
"*"			{ Line_pos += yyleng; return '*'; }
"+"			{ Line_pos += yyleng; return '+'; }
","			{ Line_pos += yyleng; return ','; }
"-"			{ Line_pos += yyleng; return '-'; }
"/"			{ Line_pos += yyleng; return '/'; }
":"			{ Line_pos += yyleng; return ':'; }
";"			{ Line_pos += yyleng; return ';'; }
"<"			{ Line_pos += yyleng; return '<'; }
"<<"			{ Line_pos += yyleng; return LSHIFT; }
"<="			{ Line_pos += yyleng; return LE; }
"=="			{ Line_pos += yyleng; return EQ; }
">"			{ Line_pos += yyleng; return '>'; }
">="			{ Line_pos += yyleng; return GE; }
">>"			{ Line_pos += yyleng; return RSHIFT; }
"?"			{ Line_pos += yyleng; return '?'; }
"^"			{ Line_pos += yyleng; return '^'; }
"{"			{ Line_pos += yyleng; return '{'; }
"|"			{ Line_pos += yyleng; return '|'; }
"||"			{ Line_pos += yyleng; return LOGOR; }
"}"			{ Line_pos += yyleng; return '}'; }
"~"			{ Line_pos += yyleng; return '~'; }

"$$"			{
			    Line_pos += yyleng;
			    rh_lval.value = getuid();
			    return NUMBER;
			}

"$"[a-zA-Z0-9]+		{
			    user_info *uip;

			    Line_pos += yyleng;
			    if (yyleng == 1) {
				rh_error("user name missing");
			    }
			    uip = getuinam(yytext + 1);
			    if (uip == (user_info *) NULL) {
				rh_error("no such user");
			    }
			    rh_lval.value = uip->uid;
			    return NUMBER;
			}

"@@"			{
			    Line_pos += yyleng;
			    rh_lval.value = getgid();
			    return NUMBER;
			}

"@"[a-zA-Z0-9]*		{
			    group_info *gip;

			    Line_pos += yyleng;
			    if (yyleng == 1) {
				rh_error("group name missing");
			    }
			    gip = getginam(yytext + 1);
			    if (gip == (group_info *) NULL) {
				rh_error("no such group");
			    }
			    rh_lval.value = gip->gid;
			    return NUMBER;
			}

[1-9][0-9]*[KMGmhdw]?	{
			    char *end;

			    Line_pos += yyleng;
			    rh_lval.value = strtol(yytext, &end, 10);

			    switch (*end) {

			    case 'G':
				rh_lval.value *= 1024 * 1024 * 1024;
				break;

			    case 'K':
				rh_lval.value *= 1024;
				break;

			    case 'M':
				rh_lval.value *= 1024 * 1024;
				break;

			    case 'd':
				rh_lval.value *= 60 * 60 * 24;
				break;

			    case 'h':
				rh_lval.value *= 60 * 60;
				break;

			    case 'm':
				rh_lval.value *= 60;
				break;

			    case 'w':
				rh_lval.value *= 60 * 60 * 24 * 7;
				break;
			    }
			    return NUMBER;
			}

0[0-7]*			{
			    Line_pos += yyleng;
			    rh_lval.value = strtol(yytext, (char **) NULL, 0);
			    return NUMBER;
			}

0[xX][0-9a-fA-F]+	{
			    Line_pos += yyleng;
			    rh_lval.value = strtol(yytext, (char **) NULL, 0);
			    return NUMBER;
			}

[a-zA-Z_][a-zA-Z0-9_]*	{
			    Line_pos += yyleng;
			    return lookup(yytext);
			}

\"			{
			    Start_line_pos = Line_pos + 1;
			    Line_pos += yyleng;
			    BEGIN(FILE_NAME);
			}

\[[\t ]*		{
			    char *p;

			    Start_line_pos = Line_pos + 1;
			    for (p = yytext; *p != '\0'; p++) {
				if (*p == '\t') {
				    Line_pos += (8 - (Line_pos + 1) % 8);
				}
				else {
				    Line_pos++;
				}
			    }
			    BEGIN(DATE_SPEC);
			}

"/*"			{
			    Start_line_pos = Line_pos + 1;
			    Start_line_num = Line_num;
			    Start_line = SAVESTR(Line);
			    Line_pos += yyleng;
			    BEGIN(COMMENT);
			}

"//"[^\n]*		{ }	/* simple comment - ignore to end of line */

"`"[a-zA-Z0-9.]+	{
			    Line_pos += yyleng;
			    rh_lval.str = SAVESTR(yytext);
			    return FSTYPE;
			}

[\t\f ]+		{			/* ignore white space */
			    char *p;

			    for (p = yytext; *p != '\0'; p++) {
				if (*p == '\f') {
				    Line_num++;
				    Line_pos = 0;
				    *Line = '\0';
				}
				else if (*p == ' ') {
				    Line_pos++;
				}
				else {
				    Line_pos += (8 - (Line_pos + 1) % 8);
				}
			    }
			}

"#"			{
			    Start_line_pos = Line_pos + 1;
			    Line_pos += yyleng;
			    BEGIN(FILE_TIME);
			}

\n			{ OldSC = INITIAL; BEGIN(NEW_LINE); }

.			{ Line_pos += yyleng; return yytext[0]; }

<<EOF>>			{ yyterminate(); }

%%

static void lex_error(const char *line, int line_num, const char *msg)
{
    (void) fprintf(stderr, "%s: %s: %d: %s\n",
		   Program_name, Input_name, line_num, msg);
    (void) fprintf(stderr, "Line: %s\nError: %*s\n",
		   line, Start_line_pos, "^");
    exit(1);
    return;
}

void rh_lex_finish(void)
{
    if (Line) {
	free(Line);
	Line = (char *) NULL;
	Line_len = Line_num = Line_pos = 0;
    }
    First_line = 1;
    yy_delete_buffer(YY_CURRENT_BUFFER);
    return;
}

static symbol *insertname(const char *s,
			  int	t,
			  long	val
			  )
{
    symbol *	sym;


    sym = ALLOCATE(sizeof(symbol));
    sym->name = SAVESTR(s);
    sym->type = t;
    sym->value = val;
    sym->req_stat = FALSE;
    sym->func = UNUSED_FUNC;
    sym->ref_count = 0;
    sym->num_params = 0;
    sym->tree = (node *) NULL;

    sym->next = Symbol_table;
    Symbol_table = sym;
    
    return (sym);
}

static symbol *locatename(const char *s)
{
    symbol *	p;


    for (p = Symbol_table; p != (symbol *) NULL; p = p->next) {
	if (strcmp(s, p->name) == 0) {
	    return (p);
	}
    }
    return ((symbol *) NULL);
}

static int lookup(const char *str)
{
    symbol *	sym;

#if defined(DEBUG2)
    (void) fprintf(stderr, "lookup(%s)\n", str);
#endif

    sym = locatename(str);
    if (sym == (symbol *) NULL) {
	sym = insertname(str, IDENTIFIER, 0L);
    }
    if (sym->type == NUMBER) {
	rh_lval.value = sym->value;
    }
    else {
	rh_lval.sym = sym;
    }
    return sym->type;
}

static void remove_escapes(char *str)
{
    char *from;
    char *to;
    
    for (to = from = str; *from; /*void*/) {
	if (*from == '\\') {
	    if (*++from) {
		*to++ = *from++;
	    }
	    else {
		*to++ = '\\';
	    }
	}
	else {
	    *to++ = *from++;
	}
    }

    *to = *from;

    return;
}

static int rh_read(char *buf,
		   int max_size
		   )
{
    int result;

    if (Input_file == -1) {
	result = (max_size < (Input_expr_end - Input_expr_ptr))
	    ? max_size : (Input_expr_end - Input_expr_ptr);
	if (result > 0) {
	    (void) memcpy(buf, Input_expr_ptr, result);
	    Input_expr_ptr += result;
	}
#if defined(DEBUG)
	else {
	    assert(result == 0);
	}
#endif
    }
    else {
	result = read(Input_file, buf, max_size);
	if (result < 0) {
	    error("read from '%s' failed : %m", Input_name);
	}
    }

    return result;
}

static date_info get_file_time(char *str)
{
    int		field;
    date_info	file_time;
    char *	from;
    int		stat_error;
    struct stat statb;
    char *	to;
    char *	type = (char *) NULL;

    for (to = from = str; *from != '\0'; /* void */) {
	if (*from == '\\') {
	    if (*++from) {
		*to++ = *from++;
	    }
	    else {
		*to++ = '\\';
	    }
	}
	else if (*from == '(') {
	    for (type = ++from; *from != '\0' && *from != ')'; ++from) {
		;	/* empty loop */
	    }
	    if (*from != ')'
		|| *(from + 1) != '\0') {
		lex_error(Line, Line_num, "invalid file time operator");
	    }
	    *from++ = '\0';		/* terminate type */
	    
	    if (*from != '\0') {	/* must be at end of str */
	    }
	    break;
	}
	else {
	    *to++ = *from++;
	}
    }

    if (type == (str + 1)) {
	lex_error(Line, Line_num, "no file specified with file time operator");
    }	

    *to = '\0';

    if (type == (char *) NULL) {
	field = RH_MTIME;
    }
    else {
	if (strcasecmp(type, "atime") == 0) {
	    field = RH_ATIME;
	}
	else if (strcasecmp(type, "ctime") == 0) {
	    field = RH_CTIME;
	}
	else if (strcasecmp(type, "mtime") == 0) {
	    field = RH_MTIME;
	}
	else {
	    lex_error(Line, Line_num, "invalid file time field");
	}
	*type = '\0';
    }

    if (stat(str, &statb) == -1) {
	stat_error = errno;
	(void) fprintf(stderr, "%s: %s: %d: file time operator file - '%s'\n",
		       Program_name, Input_name, Line_num,
		       strerror(stat_error));
	(void) fprintf(stderr, " Line: %s\nError: %*s\n",
		       Line, Start_line_pos + 1, "^");
	exit(1);
    }

    switch (field) {

    case RH_ATIME:
	file_time.seconds = statb.st_atime;
	break;

    case RH_CTIME:
	file_time.seconds = statb.st_ctime;
	break;

    case RH_MTIME:
	file_time.seconds = statb.st_mtime;
	break;
    }

    return file_time;
}
int yywrap(void)
{
    return 1;
}

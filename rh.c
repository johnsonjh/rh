/*
 * rh.c
 *
 * Written by:	Ken Stauffer
 *		Rick Ohnemus	(rick@sterling.com)
 * 
 * Global Functions: main, func_name, activate, deactivate
 *
 * Static Functions: printhelp, max_descriptors, fmt_req_stat,
 *		     prog_req_stat, execute, exam1, exam2,
 *		     exam3, exam4, get_rhpath, get_func,
 *		     push_instr, write_tree, write_functions
 */
#include "rh.h"
#include "rhcmds.h"
#include "rhdata.h"
#include "rhgram.h"
#include "alloc.h"
#include "errmsg.h"

#include <fcntl.h>
#include <pwd.h>

#if defined(DEBUG)
#  include <assert.h>
#  include "dump.h"
#endif

#define VERSION "Version 3.0r"

extern void	ftrw(char *file,
		     void (*fn)(),
		     int depth,
		     int dashr,
		     int samefsPC);
extern char *	graphic(const char *name);
extern void	printentry(int verbose,
			   int str_owner,
			   int full_date,
			   struct stat *buf,
			   const char *name);
extern void	printformat(struct stat *buf,
			    const char *name,
			    int depth);
extern int	rh_parse();
extern void	rhfinish(void);
extern void	rhinit(void);


static int *	active_funcs = (int *) NULL;
static int	num_active_funcs = 0;

#if defined(DEBUG)
extern int	gd_debug;
extern int	rh_debug;
extern int	yy_flex_debug;

int		Trace = 0;
#endif

#if defined(S_IFLNK)

#if defined(DEBUG)
#define SWITCHES "0CDFLTUVWXYZbcde:f:hilp:qrstvwx:"
#else
#define SWITCHES "0CDFLVbcde:f:hilp:qrstvwx:"
#endif

static char *usage =
    "Usage: %s [-0CDFLVbcdhilqrsvw] [-f filename] [-e expression]\n"
    "\t\t[-p format] [-x command] [directory/file ...]\n";

#else

#if defined(DEBUG)
#define SWITCHES "0CTUVWXYZbcde:f:hilp:qrstvwx:"
#else
#define SWITCHES "0CVbcde:f:hilp:qrstvwx:"
#endif

static char *usage = 
    "Usage: %s [-0CVbcdhilqrsvw] [-f filename] [-e expression]\n"
    "\t\t[-p format] [-x command] [directory/file ...]\n";
#endif


/*
 * printhelp:
 *	Print out the help screen. The string 's' is argv[0].
 *	Called when the -h option is used.
 */

static void printhelp(const char *s)
{
    int			i;
    const symbol *	p;

    (void) fprintf(stderr, usage, s);
    (void) fprintf(stderr, "\n%s\n", VERSION);
    (void) fprintf(stderr, "\noptions:\n"
#if defined(DEBUG)
		   "\t-T\ttrace\n"
		   "\t-U\tdump entire compiled program\n"
		   "\t-W\tdump executable compiled program\n"
		   "\t-X\tgetdate debug\n"
		   "\t-Y\trhgram debug\n"
		   "\t-Z\trhlex debug\n"
#endif
#if defined(S_IFLNK)
		   "\t-D\tfollow symbolic links that point to directories\n"
		   "\t-F\tfollow symbolic links that point to non-directories\n"
		   "\t-L\tfollow all symbolic links\n"
#endif
		   "\t-C\tdo not cross file systems\n"
		   "\t-V\twrite version number to stderr and exit\n"
		   "\t-b\tprint leading \"./\" if no directory\n"
		   "\t-c\tdo not read RHRC file\n"
		   "\t-d\tleading '?' or '*' matches leading '.' in file name\n"
		   "\t-e\tget expression from the command line\n"
		   "\t-f\tget expression from file\n"
		   "\t-i\tignore case during file name matching\n"
		   "\t-h\tshow this message and exit\n"
		   "\t-l\tlong filename output\n"
		   "\t-p format\tprint file information using 'format'\n"
		   "\t-q\tdisplay non-graphic characters in filenames as ?\n"
		   "\t-r\tdo not go down the directory tree (non-recursive)\n"
		   "\t-s\tprint users/groups as strings instead of numbers\n"
		   "\t-t\tprint full date/time instead of ls(1) type date/time\n"
		   "\t-v\tverbose output\n"
		   "\t-w\tdisplay warning messages\n"
		   "\t-x command\texecute 'command' for matching files\n"
		   "\t-0\tseparate filenames by null char, not newline\n"
		   "\nvalid symbols:\n"
		   );
		   
    for (i = 1, p = Symbol_table; p != (symbol *) NULL; p = p->next, i++) {
	(void) fprintf(stderr, "%12s%s", p->name,
		      (((i - 1) % 5) == 4
		       || p->next == (symbol *) NULL) ? "\n" : " ");
    }
    
    (void) fprintf(stderr, "\nC operators:\n"
		   "\t! ~ - * / %% + < <= > >= == != & ^ | << >> && || ?:\n"
		   "\nspecial operators:\n"
		   "\t$username , @groupname , \"*.c\" , [date/time spec]\n\n"
		   );
    
    exit(1);
}

#if defined(RLIMIT_NOFILE)

/*
 * max_descriptors:
 *	Returns the maximum number of file descriptors.
 */

static int max_descriptors(void)
{
    struct rlimit newrlp;
    struct rlimit rlp;
    
    if (getrlimit(RLIMIT_NOFILE, &rlp) == 0) {
	if (rlp.rlim_cur < NOFILE) {
	    newrlp.rlim_max = newrlp.rlim_cur = NOFILE;
	    if (setrlimit(RLIMIT_NOFILE, &newrlp) == 0)
		return NOFILE;
	}
	return rlp.rlim_cur;
    }
    else {
	return NOFILE;
    }
}
#endif

/*
 * func_name:
 *	If a PC corresponds to a compiled function then
 *	return the function's name. Otherwise, return
 *	NULL.
 */

const char *func_name(int pc)
{
    const symbol *	s;

    for (s = Symbol_table; s != (symbol *) NULL; s = s->next) {
	if (s->type == FUNCTION && s->value == pc) {
	    return s->name;
	}
    }

    return (char *) NULL;
}

/*
 * activate:
 *	If a function is active then return 0 (FALSE). If the function
 *	is not active then save the pc and return 1 (TRUE).
 */

int activate(int pc)
{
    int		i;

    for (i = 0; i < num_active_funcs; i++) {
	if (pc == active_funcs[i]) {
	    return 0;
	}
    }

    active_funcs =
	REALLOCATE(active_funcs, (num_active_funcs + 1) * sizeof(int));

    active_funcs[num_active_funcs++] = pc;
    return 1;
}

/*
 * deactivate:
 *	Remove a function's pc from the active list.
 */

void deactivate(int pc)
{
    const char *	fn;
    int			i;

    for (i = 0; i < num_active_funcs; i++) {
	if (pc == active_funcs[i]) {
	    if (num_active_funcs == 1) {
		num_active_funcs = 0;
		free((char *) active_funcs);
		active_funcs = (int *) NULL;
	    }
	    else {
		for (--num_active_funcs; i < num_active_funcs; i++) {
		    active_funcs[i] = active_funcs[i + 1];
		}
	    }
	    return;
	}
    }

    fn = func_name(pc);
    error("deactivate: function '%s' not active\n", (fn == NULL) ? "" : fn);
}

/*
 * fmt_req_stat:
 *	Scan '-p' switch format string for anything that
 *	requires a stat() call to be made.
 *	Return TRUE if stat() is needed or FALSE if it isn't.
 */

static int fmt_req_stat(void)
{
    const char *	fp;
    int			stat_all = FALSE;

    for (fp = attr.format; *fp != '\0' && !stat_all; fp++) {
	if (*fp != '%') {
	    continue;
	}

	++fp;

	switch (*fp) {
	case '\0':
	    error("format character missing after '%%'");
	    /*NOTREACHED*/
	    
	case 'a':
#if BSD
	case 'b':
	case 'B':
#endif
	case 'c':
	case 'g':
	case 'G':
	case 'i':
	case 'l':
	case 'm':
	case 'p':
	case 'P':
	case 'r':
	case 'R':
	case 'u':
	case 'U':
	case 'w':
	case 'W':
	case 'y':
	case 'z':
	    stat_all = TRUE;
	    break;
	}
    }

    return stat_all;
}

/*
 * prog_req_stat:
 *	Scan the compiled program for any builtin functions
 *	that require a stat() call to be made.
 *	Return TRUE if stat() is needed or FALSE if it isn't.
 */

static int prog_req_stat(int pc)
{
    const instruction *	ip;
    const symbol *	sym;

    if (StackProgram[pc].func == END_OF_PROG) {
	return -1;
    }

    for (ip = &StackProgram[pc]; ip->func != NULL_INSTR; ip++, pc++) {
	for (sym = Symbol_table; sym != (symbol *) NULL; sym = sym->next) {
	    if (ip->func == sym->func) {
		if (sym->req_stat) {
		    return 1;
		}
		break;
	    }
	}

	if (ip->func == c_func) {
	    if (activate(ip->value.pc)) {
		if (prog_req_stat((int) ip->value.pc)) {
		    deactivate(ip->value.pc);
		    return 1;
		}
		deactivate(ip->value.pc);
	    }
	}
    }

    return 0;
}

/*
 * execute:
 *	Execute the program contained in the StackProgram[] array.
 *	Each element of the StackProgram[] array contains a pointer
 *	to a C function.
 *	Programs are NULL_INSTR terminated. (NULL_INSTR is a NULL
 *	function pointer.)
 *	Returns the result of the expression.
 */

static long execute(void)
{
    SP = 0;

    if (startPC == -1) {
	return 1;
    }

    for (PC = startPC; StackProgram[PC].func != NULL_INSTR; PC++) {
#if defined(DEBUG)
	if (Trace) {
	    dump_instr(PC, &StackProgram[PC]);
	}
#endif
	(*StackProgram[PC].func)(&StackProgram[PC].value);
	if (SP < 0) {
	    error("data stack underflow");
	}
	else if (SP >= MAX_STACK_LEN) {
	    error("data stack overflow");
	}
    }
#if defined(DEBUG)
    assert(SP == 1);
    if (Trace) {
	(void) fprintf(stderr, "execute() returning %ld\n", Stack[0]);
    }
#endif
    return Stack[0];
}

/*
 * exam1:
 * exam2:
 * exam3:
 * exam4:
 *	One of these functions is called for every file that 'rh'
 *	examines. Each function first calls execute() to see if the
 *	expression is TRUE. If the expression is TRUE (non-zero)
 *	then the action specified by the command line options is
 *	executed.
 */

/* print file out by itself */

static void exam1(void)
{
    if (execute()) {
	(void) printf("%s%c",
	    attr.graphic ? graphic(attr.fname) : attr.fname,
	    attr.zero_term ? '\0' : '\n');
	if (attr.zero_term) {
	    (void)fflush(stdout);
	}
    }
}

/* long output of file */

static void exam2(void)
{
    if (execute()) {
	printentry(attr.verbose, attr.str_owner, attr.full_date,
		   attr.buf, attr.fname);
    }
    return;
}

/* formatted output of file information */

static void exam3(void)
{
    if (execute()) {
	printformat(attr.buf, attr.fname, attr.depth);
    }
    return;
}

/* do a system(3) call of desired command */

static void exam4(void)
{
    char        command[NCARGS];
    char *      end;
    int         f_l;
    char *      n;
    int         n_l;
    char *      p;
    char *      q;
    char *      r;
    int         rv;
    
    if (execute()) {
	p = command;
	end = command + sizeof(command);
	n = (char *) NULL;
	q = attr.command;
	f_l = strlen(attr.fname);
	while (*q != '\0') {
	    if (*q != '%') {
		*p++ = *q++;
	    }
	    else {
		++q;
		if (*q == 's') {
		    if ((p + f_l) >= end) {
			warning("%s: maximum command line length exceeded",
				attr.fname);
			return;
		    }
		    r = attr.fname;
		    while ((*p++ = *r++) != '\0')
			/* void */ ;
		    p--;
		}
		else if (*q == 'S') {
		    if (n == (char *) NULL) {
			if ((n = strrchr(attr.fname, '/')) == (char *) NULL) {
			    n = attr.fname;
			}
			else {
			    ++n;
			}
			n_l = strlen(n);
		    }
		    if ((p + n_l) >= end) {
			warning("%s: maximum command line length exceeded",
				attr.fname);
			return;
		    }
		    r = n;
		    while ((*p++ = *r++) != '\0')
			/* void */ ;
		    p--;
		}
		else {
		    if ((p + 2) >= end) {
			warning("%s: maximum command line length exceeded",
				attr.fname);
			return;
		    }
		    *p++ = '%';
		    *p++ = *q;
		}
		++q;
	    }
	    if (p == end) {
		warning("%s: maximum command line length exceeded",
			attr.fname);
		return;
	    }
	}
	*p = '\0';
	rv = system(command);
	if (rv != 0) {
	    ++attr.num_errors;
	}
	if (attr.verbose) {
	    (void) printf("%s exit(%d)\n", command, rv);
	}
    }
    
    return;
}

/*
 * get_rhpath:
 *	Return value of RHPATH environment variable.
 *	If RHPATH is not set then attempt to return
 *	the user's home directory.
 *
 *	The value is returned in allocated memory.
 */

static char *get_rhpath(void)
{
    const char *	home;
    char *		rhpath;

    rhpath = getenv(RHPATH_ENV);
    if (rhpath == (char *) NULL) {
	home = getenv(HOME_ENV);
	if (home == (char *) NULL) {
	    struct passwd *	info;
	    
	    info = getpwuid(getuid());
	    if (info == (struct passwd *) NULL) {
		rhpath = SAVESTR(".");
	    }
	    else {
		rhpath = ALLOCATE(strlen(info->pw_dir) + 3);
		(void) sprintf(rhpath, ".:%s", info->pw_dir);
	    }
	}
	else {
	    rhpath = ALLOCATE(strlen(home) + 3);
	    (void) sprintf(rhpath, ".:%s", home);
	}
    }
    else {
	rhpath = SAVESTR(rhpath);
    }

    return rhpath;
}

/*
 * get_func:
 *	Returns the symbol table entry for an rh
 *	language function given the name of the function.
 */

symbol *get_func(char *func_name)
{
    symbol *	sym;

    for (sym = Symbol_table; sym != (symbol *) NULL; sym = sym->next) {
	if (sym->type == FUNCTION && strcmp(sym->name, func_name) == 0) {
	    return sym;
	}
    }
    
    error("function '%s' lookup failure", func_name);

    /*NOTREACHED*/
}

/*
 * push_instr:
 *	Pushes an instruction on to the program stack.
 */

static void push_instr(void	      (*func)(instr_value *),
		       instr_value *	value)
{
    if (PC >= MAX_PROG_LEN) {
	error("program is too big");
    }
    StackProgram[PC].func = func;
    StackProgram[PC++].value = *value;
    return;
}

/*
 * write_tree:
 *	Compiles an expression tree on the program stack.
 */

static void write_tree(tree)
node *	tree;
{
    instr_value	value;

    if (tree == (node *) NULL) {
	return;
    }

    if (tree->code.func == UNRES_FUNC) {
	symbol *sym = get_func(tree->code.value.func_name);
	if (tree->count > 0) {
	    int i;
	    
	    for (i = 0; i < tree->count; i++) {
		write_tree(tree->trees[i]);
	    }
	    free((char *) tree->trees);
	}
	tree->count = 0;
	if (sym->value == -1) {
	    push_instr(UNRES_FUNC, &tree->code.value);
	}
	else {
	    value.pc = sym->value;
	    push_instr(c_func, &value);
	}
    }
    else if (tree->code.func == TERTIARY) {
	int colon_pc;
	int qm_pc;

	write_tree(tree->left);
	qm_pc = PC;
	value.pc = 0;
	push_instr(c_qm, &value);
	write_tree(tree->middle);
	colon_pc = PC;
	push_instr(c_colon, &value);
	write_tree(tree->right);
	StackProgram[qm_pc].value.pc = colon_pc;
	StackProgram[colon_pc].value.pc = PC - 1;
    }
    else if (tree->code.func == c_syscall) {
	if (tree->count > 0) {
	    int i;
	    
	    for (i = 0; i < tree->count; i++) {
		write_tree(tree->trees[i]);
	    }
	    free((char *) tree->trees);
	}
	tree->count = 0;
	push_instr(c_syscall, &tree->code.value);
    }
    else {
	write_tree(tree->left);
	write_tree(tree->right);
	push_instr(tree->code.func, &tree->code.value);
    }

    free((char *) tree);

    return;
}

/*
 * write_functions:
 *	Compiles all rh language functions on to the program stack.
 */

static void write_functions(void)
{
    instruction *	ip;
    symbol *		sym;
    instr_value		value;

    for (sym = Symbol_table; sym != (symbol *) NULL; sym = sym->next) {
	if (sym->type == FUNCTION) {
	    sym->value = PC;

	    value.num_params = sym->num_params;
	    push_instr(NUM_PARAMS, &value);

	    write_tree(sym->tree);

	    value.num_params = sym->num_params;
	    push_instr(c_return, &value);

	    value.l = 0L;
	    push_instr(NULL_INSTR, &value);

	    sym->tree = (node *) NULL;
	}
    }

    value.l = 0L;
    push_instr(NULL_INSTR, &value);
    push_instr(END_OF_PROG, &value);
    PC--;

    for (ip = StackProgram; ip->func != END_OF_PROG; ip++) {
	if (ip->func == UNRES_FUNC) {
	    ip->func = c_func;
	    sym = get_func(ip->value.func_name);
	    ip->value.pc = sym->value;
	}
    }

    return;
}

/*
 * main:
 *	Parse arguments.
 *	  -b, -c, -D, -d, -F, -i, -L, -l, -q, -r, -s, -t, and -w switches
 *	  can occur as often as desired even though only one occurence
 *	  is necessaty.
 *
 *	  -e and -f can occur one or more times and each occurence must
 *	  have an associated argument.
 *
 *	  First occurence of -h or -V causes a message to be output then
 *	  rh exits.
 *
 *	  -p and -x can occur only once and must have an associated argument.
 *
 *	  -T, -W, -X, -Y, and -Z are only available if rh is compiled with
 *	  debug turned on.
 *
 *      Parse any RHRC file.
 *	Parse any -f filenames.
 *	Parse any -e expressions.
 *	If there is a start expression then compile it and all rh language
 *	functions.
 *	Perform the recursive hunt on remaining arguments.
 */

int
main(int argc, char *argv[])
{
    extern int		getopt();
    extern char *	optarg;
    extern int		opterr;
    extern int		optind;
    extern char *	resolve_name(const char *paths, const char *name);
    extern void		yyrestart();

    char **		cl_exprs = (char **) NULL;
    char **		cl_fnames = (char **) NULL;
    int			colon_pc;
    int			dashb = FALSE;
    int			dashl = FALSE;
    int			dashr = TRUE;
    int			depth;
    int			dont_cross_filesystems = FALSE;
    void	      (*examptr)(void) = exam1;	/* default output function */
    int			first_parse = TRUE;
    int			i;
    int			num_cl_exprs = 0;
    int			num_cl_fnames = 0;
    int			qm_pc;
    int			read_rhrc = TRUE;
    char *		rhpath = (char *) NULL;	/* RHPATH env. variable */
    int			samefsPC = -1;
    instr_value		value;

#if defined(DEBUG)
    int			dump_all;
    int			dump_exec;
#endif


    Program_name = strrchr(argv[0], '/');
    Program_name = (Program_name == NULL) ? argv[0] : (Program_name + 1);

#if defined(DEBUG)
    dump_all = dump_exec = gd_debug = rh_debug = yy_flex_debug = 0;
#endif

    attr.command = (char *) NULL;	/* -x option */
    attr.format = (char *) NULL;	/* -p option */
    attr.dot_special = TRUE;		/* -d option */
#if defined(S_IFLNK)
    attr.follow_dir = FALSE;		/* -D & -L options */
    attr.follow_file = FALSE;		/* -F & -L options */
#endif
    attr.graphic = FALSE;		/* -q option */
    attr.ignore_case = FALSE;		/* -i option */
    attr.prune = FALSE;			/* prune (obviously :-) */
    attr.stat_all = FALSE;		/* don't need to stat everything */
    attr.str_owner = FALSE;		/* -u option */
    attr.verbose = FALSE;		/* -v option */
    attr.full_date = FALSE;		/* -t option */
    attr.num_errors = 0;		/* no errors yet */


    rhinit();

    opterr = 1;

    while ((i = getopt(argc, argv, SWITCHES)) != EOF) {

	switch (i) {
	    
	case 'b':
	    dashb = TRUE;
	    break;
	    
	case 'C':
	    dont_cross_filesystems = TRUE;
	    attr.stat_all = TRUE;	/* <<< is this necessary? */
	    break;

	case 'c':
	    read_rhrc = FALSE;
	    break;

#if defined(S_IFLNK)
	case 'D':
	    attr.follow_dir = TRUE;
	    attr.stat_all = TRUE;
	    break;
#endif
	    
	case 'd':
	    attr.dot_special = FALSE;
	    break;
	    
	case 'e':
	    cl_exprs =
		REALLOCATE(cl_exprs, sizeof(char *) * (num_cl_exprs + 1));
	    cl_exprs[num_cl_exprs++] = optarg;
	    break;
	    
#if defined(S_IFLNK)
	case 'F':
	    attr.follow_file = TRUE;
	    attr.stat_all = TRUE;
	    break;
#endif
	    
	case 'f':
	    cl_fnames =
		REALLOCATE(cl_fnames, sizeof(char *) * (num_cl_fnames + 1));
	    cl_fnames[num_cl_fnames++] = optarg;
	    break;
	    
	case 'h':
	    printhelp(argv[0]);
	    /*NOTREACHED*/
	    
	case 'i':
	    attr.ignore_case = TRUE;
	    break;
	    
#if defined(S_IFLNK)
	case 'L':
	    attr.follow_dir = TRUE;
	    attr.follow_file = TRUE;
	    attr.stat_all = TRUE;
	    break;
#endif
	    
	case 'l':
	    examptr = exam2;
	    dashl = TRUE;
	    attr.stat_all = TRUE;
	    break;
	    
	case 'p':
	    if (attr.format != (char *) NULL) {
		error("only one -p option is allowed");
	    }
	    attr.format = optarg;
	    examptr = exam3;
	    break;
	    
	case 'q':
	    attr.graphic = TRUE;
	    break;
	    
	case 'r':
	    dashr = FALSE;
	    break;
	    
	case 's':
	    attr.str_owner = TRUE;
	    break;
	    
	case 't':
	    attr.full_date = TRUE;
	    break;

	case 'V':
	    error(VERSION);
	    /*NOTREACHED*/
	    
	case 'v':
	    attr.verbose = TRUE;
	    break;
	    
	case 'w':
	    Issue_warnings = TRUE;
	    break;

	case 'x': 
	    if (attr.command != (char *) NULL) {
		error("only one -x option is allowed");
	    }
	    examptr = exam4;
	    attr.command = optarg;
	    break;

	case '0':
	    attr.zero_term = TRUE;
	    break;
	    
#if defined(DEBUG)
	case 'T':
	    ++Trace;
	    break;

	case 'U':
	    ++dump_all;
	    break;

	case 'W':
	    ++dump_exec;
	    break;

	case 'X':
	    ++gd_debug;
	    break;

	case 'Y':
	    ++rh_debug;
	    break;

	case 'Z':
	    ++yy_flex_debug;
	    break;

#endif

	default:
	    warning("use -h for help");
	    exit(1);
	}
	
    }
    
    /* perform a little bit of sanity checking on switch combinations */

    if (attr.command != (char *) NULL) {
	if (dashl) {
	    error("cannot have both -x and -l options");
	}
	else if (attr.format != (char *) NULL) {
	    warning("-x overrides -p, -p ignored");
	    attr.format = (char *) NULL;
	}
    }
    else if (attr.format != (char *) NULL) {
	if (dashl) {
	    warning("-p overrides -l, -l ignored");
	    dashl = FALSE;
	}
	if (attr.verbose) {
	    warning("-p overrides -v, -v ignored");
	    attr.verbose = FALSE;
	}
    }

    /*
     * Always display control characters in file names as a '?'
     * if the output is a terminal.
     */

    if (isatty(fileno(stdout))) {
	attr.graphic = TRUE;
    }
    
    /*
     * Read and parse functions/expression in files.
     */

    startPC = -1;
    
    if (read_rhrc || num_cl_fnames) {
	rhpath = get_rhpath();
	if (read_rhrc) {
	    char *rhrc = getenv(RHRC_ENV);
	    if (rhrc == (char *) NULL) {
		rhrc = RHRC;
	    }
	    Input_name = resolve_name(rhpath, rhrc);
	    Input_file = open(Input_name, O_RDONLY, 0);
	    if (Input_file != -1) {
		if (rh_parse()) {
		    exit(1);
		}
		first_parse = FALSE;
		(void) close(Input_file);
	    }
	    free(Input_name);
	}
	if (num_cl_fnames) {
	    for (i = 0; i < num_cl_fnames; i++) {
		if (!first_parse) {
		    yyrestart(stdin);
		}
		if (strcmp(cl_fnames[i], "-") == 0) {
		    Input_name = "stdin";
		    if (feof(stdin)) {
			Input_file = open("/dev/tty", O_RDONLY, 0);
			if (Input_file == -1)  {
			    error("can't reopen stdin : %m");
			}
			if (rh_parse()) {
			    exit(1);
			}
			(void) close(Input_file);
		    }
		    else {
			Input_file = fileno(stdin);
			if (rh_parse()) {
			    exit(1);
			}
		    }
		}
		else {
		    Input_name = resolve_name(rhpath, cl_fnames[i]);
		    Input_file = open(Input_name, O_RDONLY, 0);
		    if (Input_file == -1) {
			error("can't open %s : %m", Input_name);
		    }
		    if (rh_parse()) {
			exit(1);
		    }
		    (void) close(Input_file);
		    free(Input_name);
		}
		first_parse = FALSE;
	    }
	    free((char *) cl_fnames);
	}
    }

    if (rhpath != (char *) NULL) {
	free(rhpath);
    }
    
    /*
     * Parse expressions from -e switches
     */

    if (num_cl_exprs) {
	Input_file = -1;
	Input_name = "cmd line";
	for (i = 0; i < num_cl_exprs; i++) {
	    Input_expr_ptr = cl_exprs[i];
	    Input_expr_end = Input_expr_ptr + strlen(Input_expr_ptr);
	    if (!first_parse) {
		yyrestart(stdin);
	    }
	    if (rh_parse()) {
		exit(1);
	    }
	    first_parse = FALSE;
	}
	free((char *) cl_exprs);
    }

    /*
     * Compile the parse tree if a start expression was specified.
     */

    if (Start_expr == (node *) NULL) {
	if (Issue_warnings) {
	    warning("no start expression specified");
	}
	if (dont_cross_filesystems) {
	    samefsPC = startPC = PC = 0;

	    value.dev = (dev_t) 0;
	    push_instr(c_samefs, &value);

	    qm_pc = PC;
	    value.pc = 0;
	    push_instr(c_qm, &value);

	    value.l = 1L;
	    push_instr(c_number, &value);

	    colon_pc = PC;
	    value.pc = 0;
	    push_instr(c_colon, &value);

	    value.l = 0L;
	    push_instr(c_prune, &value);

	    StackProgram[qm_pc].value.pc = colon_pc;
	    StackProgram[colon_pc].value.pc = PC - 1;

	    value.l = 0L;
	    push_instr(NULL_INSTR, &value);
	    push_instr(END_OF_PROG, &value);
	}
    }
    else {
	PC = 0;
	write_functions();
	startPC = PC;
	if (dont_cross_filesystems) {
	    samefsPC = PC;
	    value.dev = (dev_t) 0;
	    push_instr(c_samefs, &value);

	    qm_pc = PC;
	    value.pc = 0;
	    push_instr(c_qm, &value);
	}
	write_tree(Start_expr);
	if (dont_cross_filesystems) {
	    colon_pc = PC;
	    value.pc = 0;
	    push_instr(c_colon, &value);

	    value.l = 0L;
	    push_instr(c_prune, &value);

	    StackProgram[qm_pc].value.pc = colon_pc;
	    StackProgram[colon_pc].value.pc = PC - 1;
	}
	value.l = 0L;
	push_instr(NULL_INSTR, &value);
	push_instr(END_OF_PROG, &value);
    }

#if defined(DEBUG)
    if (startPC != -1) {
	if (dump_all) {
	    int opc = 0;
	    int npc;

	    while ((npc = dump_program(opc, 0)) != -1) {
		opc = npc;
	    }
	}
	else if (dump_exec) {
	    (void) dump_program(startPC, 1);
	}
    }
#endif

    rhfinish();
    
    if (!attr.stat_all) {
	if (attr.format != (char *) NULL) {
	    attr.stat_all = fmt_req_stat();
	}
	if (!attr.stat_all && startPC != -1) {
	    /* scan program for anything that requires a stat() call */
	    attr.stat_all = prog_req_stat(startPC);
	}
    }

#if defined(DEBUG2)
    (void) fprintf(stderr, "stat all = %s", attr.stat_all ? "true" : "false");
#endif

    depth = dashr ? DEPTH : 1;
    
    if (optind >= argc) {
	ftrw(dashb ? "." : "", examptr, depth, dashr, samefsPC);
    }
    else {
	for (/* void */; optind < argc; optind++) {
	    if (samefsPC != -1) {
		StackProgram[samefsPC].value.dev = (dev_t) 0;
	    }
	    ftrw(argv[optind], examptr, depth, dashr, samefsPC);
	}
    }
    
#if defined(DEBUG)
    {
	symbol *sym;

	while (Symbol_table->type == FUNCTION) {
	    sym = Symbol_table;
	    Symbol_table = Symbol_table->next;
	    free(sym->name);
	    free((char *) sym);
	}
    }
    if (startPC != -1) {
	int pc;

	for (pc = 0; StackProgram[pc].func != END_OF_PROG; ++pc) {
	    if (StackProgram[pc].func == c_pattern
		|| StackProgram[pc].func == c_fncase
		|| StackProgram[pc].func == c_fnigncase) {
		free(StackProgram[pc].value.pattern);
	    }
	    else if (StackProgram[pc].func == c_date) {
		free((char *) StackProgram[pc].value.di);
	    }
	}
    }
#endif

    exit((attr.num_errors == 0) ? 0 : 1);
    /*NOTREACHED*/
    return(1);
}

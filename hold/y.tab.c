#ifndef lint
static const char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20120115

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)


#ifndef yyparse
#define yyparse    rh_parse
#endif /* yyparse */

#ifndef yylex
#define yylex      rh_lex
#endif /* yylex */

#ifndef yyerror
#define yyerror    rh_error
#endif /* yyerror */

#ifndef yychar
#define yychar     rh_char
#endif /* yychar */

#ifndef yyval
#define yyval      rh_val
#endif /* yyval */

#ifndef yylval
#define yylval     rh_lval
#endif /* yylval */

#ifndef yydebug
#define yydebug    rh_debug
#endif /* yydebug */

#ifndef yynerrs
#define yynerrs    rh_nerrs
#endif /* yynerrs */

#ifndef yyerrflag
#define yyerrflag  rh_errflag
#endif /* yyerrflag */

#ifndef yylhs
#define yylhs      rh_lhs
#endif /* yylhs */

#ifndef yylen
#define yylen      rh_len
#endif /* yylen */

#ifndef yydefred
#define yydefred   rh_defred
#endif /* yydefred */

#ifndef yydgoto
#define yydgoto    rh_dgoto
#endif /* yydgoto */

#ifndef yysindex
#define yysindex   rh_sindex
#endif /* yysindex */

#ifndef yyrindex
#define yyrindex   rh_rindex
#endif /* yyrindex */

#ifndef yygindex
#define yygindex   rh_gindex
#endif /* yygindex */

#ifndef yytable
#define yytable    rh_table
#endif /* yytable */

#ifndef yycheck
#define yycheck    rh_check
#endif /* yycheck */

#ifndef yyname
#define yyname     rh_name
#endif /* yyname */

#ifndef yyrule
#define yyrule     rh_rule
#endif /* yyrule */
#define YYPREFIX "rh_"

#define YYPURE 0

#line 2 "rhgram.y"

/*
 * rhgram.y
 *
 * Written by:	Rick Ohnemus	(rick@sterling.com)
 */

#if !defined(lint)
static char rcsid[] = "$Id: rhgram.y,v 1.1 2008/12/27 00:56:03 vandys Exp $";
#endif

#include "rh.h"
#include "rhcmds.h"
#include "rhdata.h"
#include "alloc.h"
#include "errmsg.h"

extern char *	Line;		/* line being parsed */
extern int	Line_num;	/* line number */
extern int	Line_pos;	/* position in line */

static node *	copy_tree(node *);	/* copy a tree */
extern void	free_tree(node *);	/* free expression tree memory */ 
static node *	make_node(void (*)(), instr_value *);	/* make a tree node */
static int	pattern(const char *);	/* is string a shell fn pattern? */
extern void	rh_error(const char *);	/* output an error message and exit */
extern int	rh_lex(void);		/* lexer */
extern void	rh_lex_finish(void);	/* lexer clean-up */

static instr_value	value;

#line 35 "rhgram.y"
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union {
    char *              str;
    symbol *		sym;
    node *		tree;
    long                value;
    void	      (*func)(instr_value *);
    date_info *		di;
    struct {
	char *	line;
	int	line_num;
	int	line_pos;
    } line_info;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
#line 152 "y.tab.c"

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define BLTIN 257
#define DATESPEC 258
#define EQ 259
#define FIELD 260
#define FILENAME_PATTERN 261
#define FILETIME_FIELD 262
#define FSTYPE 263
#define FUNCTION 264
#define GE 265
#define IDENTIFIER 266
#define LE 267
#define LOGAND 268
#define LOGOR 269
#define LSHIFT 270
#define NE 271
#define NUMBER 272
#define PARAM 273
#define RETURN 274
#define RSHIFT 275
#define SYSTEM_CALL 276
#define TIME_FIELD 277
#define UNIMINUS 278
#define YYERRCODE 256
static const short rh_lhs[] = {                          -1,
    0,    0,    0,    7,    7,    9,    8,   10,   11,    8,
    8,    5,    5,    5,    5,    4,    4,   12,   12,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,   13,    3,   14,    3,    3,    3,    3,    3,
    3,    3,    3,    3,    3,    3,    3,    6,    6,    2,
    2,    2,
};
static const short rh_len[] = {                           2,
    1,    2,    3,    0,    2,    0,    3,    0,    0,    7,
    1,    0,    1,    3,    1,    5,    4,    0,    1,    1,
    5,    3,    3,    3,    3,    3,    3,    3,    3,    3,
    3,    3,    3,    3,    3,    3,    3,    3,    3,    2,
    2,    2,    0,    3,    0,    3,    3,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    0,    3,    0,
    1,    3,
};
static const short rh_defred[] = {                        4,
    0,    0,   11,   51,   55,   52,   49,   56,   50,   43,
    0,   48,   53,   45,   57,    0,    0,    0,    0,    0,
   20,    5,    0,    0,    0,    0,   54,   42,   41,   40,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    3,    0,   44,    0,    7,    0,   46,   47,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   37,   38,   39,    0,    0,   19,
    0,   15,   13,    0,    0,   59,    0,    0,    9,    0,
    0,    0,    0,   17,    0,   14,   16,   10,
};
static const short rh_dgoto[] = {                         1,
   20,   79,   21,   55,   84,   53,    2,   22,   24,   25,
   95,   81,   23,   26,
};
static const short rh_sindex[] = {                        0,
    0,  -21,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  312,  312,  312,  312,  648,
    0,    0,  -38, -116,  -29,  -38,    0,    0,    0,    0,
  677,  312,  312,  312,  312,  312,  312,  312,  312,  312,
  312,  312,  312,  312,  312,  312,  312,  312,  312,  312,
    0,  312,    0, -258,    0, -253,    0,    0,  803,  -22,
  -22,  761,  747,  -37,  803,  -37,  706,  602,  775,  789,
  -22,  -22,  -33,  -33,    0,    0,    0,  720,  -15,    0,
  312,    0,    0,  -14,  312,    0,  312,  458,    0, -249,
  720,  720, -107,    0, -116,    0,    0,    0,
};
static const short rh_rindex[] = {                        0,
    0,   22,    0,    0,    0,    0,    0,    0,    0,    0,
   28,    0,    0,    0,    0,    0,    0,    0,    0,   47,
    0,    0,    1,    0,    0,    1,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  -10,    0,  355,    0,   -9,    0,    0,  383,  138,
  164,  261,  148,   96,  399,  109,    0,  420,  408,  410,
  291,  331,   41,   69,    0,    0,    0,   -8,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  360,   -4,    0,    0,    0,    0,    0,    0,
};
static const short rh_gindex[] = {                        0,
 1058,    0,    0,  -46,    0,   24,    0,    0,    0,    0,
    0,    0,    0,    0,
};
#define YYTABLESIZE 1145
static const short rh_table[] = {                        50,
   58,   52,   82,   50,   48,   46,   54,   47,   48,   49,
   56,   17,   83,   49,   50,   80,   96,   97,   19,   48,
   46,    1,   47,   16,   49,   86,   89,   54,   87,   90,
   60,   12,   61,   60,   12,   61,   62,   58,   58,   62,
   35,   58,   58,   58,   58,   58,    2,   58,   98,   57,
    0,    0,    0,    0,    0,    0,    0,    0,   58,   58,
   58,    0,   58,   58,   54,   54,    0,    8,   36,   54,
   54,    0,   54,    0,   54,    0,    0,    0,   35,    0,
    0,   35,    0,   35,   35,   35,   54,   54,    0,   54,
   54,    0,    0,    0,   58,   34,    0,    0,   35,   35,
   35,    0,   35,   35,   18,    0,   36,    0,   33,   36,
    0,   36,   36,   36,    0,    0,    0,    0,    0,    0,
    0,   54,    0,    0,   58,   58,   36,   36,   36,    0,
   36,   36,    0,   34,   35,    0,   34,   32,    0,   34,
    0,    0,    0,    0,    0,    0,   33,   22,    0,   33,
    6,   54,   33,   34,   34,   34,    0,   34,   34,    0,
    0,    0,   36,   31,   35,   35,   33,   33,   33,    0,
   33,   33,    0,    0,    0,   32,    0,    0,   32,    0,
    0,   32,    0,    0,    0,    0,    0,    0,   22,   34,
    0,   22,   36,   36,    0,   32,   32,   32,    0,   32,
   32,   31,   33,    0,   31,   22,   22,   31,    0,    0,
   22,    0,    0,    0,    0,    0,    0,    0,    0,   34,
   34,   31,   31,   31,    0,   31,   31,    0,    0,    0,
    0,   32,   33,   33,    3,    4,    5,    0,    6,    7,
    8,    9,   10,    0,   11,    0,    0,   37,    0,    0,
   12,   13,   39,    0,   14,   15,    0,   31,    0,   58,
   23,   32,   32,    0,    0,   58,    0,   58,   58,   58,
   58,   58,   22,    0,    0,   58,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   54,   31,   31,    0,
   29,    0,   54,    0,   54,   54,   54,   54,   54,   35,
    0,   23,   54,    0,   23,   35,    0,   35,   35,   35,
   35,   35,    0,    0,    0,   35,    0,    0,   23,   23,
    0,    0,    0,   23,    0,    0,    0,   36,   29,    0,
   30,   29,    0,   36,   29,   36,   36,   36,   36,   36,
    0,    0,    0,   36,   17,    0,    0,    0,   29,   29,
   29,   19,   29,   29,   34,    0,   16,    0,    0,   21,
   34,    0,   34,   34,   34,   34,   34,   33,   30,    0,
   34,   30,    0,   33,   30,   33,   33,   33,   33,   33,
    0,    0,   27,   33,   29,   23,    0,   18,   30,   30,
   30,    0,   30,   30,   18,    0,   32,    0,   28,   18,
   21,    0,   32,   21,   32,   32,   32,   25,   32,   26,
    0,    0,    0,    0,   29,   29,   22,   21,   21,   24,
   27,    0,   31,   27,   30,    0,   27,    0,   31,    0,
   31,   31,   31,    0,   31,    0,   28,   18,    0,   28,
   27,   27,   28,    0,    0,   27,    0,   26,   25,    0,
   26,   25,    0,   26,   30,   30,   28,   28,    0,    0,
   24,   28,    0,   24,    0,   25,   25,   26,   26,    0,
   25,    0,   26,    0,    0,    0,   27,   24,   24,    0,
   18,    0,   24,    0,   21,    0,    0,    0,    0,    0,
    0,    0,   28,    0,   50,   43,    0,    0,    0,   48,
   46,   25,   47,   26,   49,    0,   27,   27,    0,    0,
    0,    0,    0,    0,    0,    0,   93,   44,    0,   45,
   40,    0,   28,   28,    0,    0,    0,    0,   23,   23,
    0,   25,   25,   26,   26,    0,    0,    0,    0,    0,
    0,    0,    0,   24,   24,    0,    0,    0,    0,   29,
    0,   42,    0,    0,    0,   29,    0,   29,   29,   29,
    0,   29,    0,    0,    0,    0,    0,    0,    4,    5,
    0,    6,    7,    8,    9,   10,    0,   27,    0,    0,
    0,   41,   94,   12,   13,    0,    0,   14,   15,   30,
    0,    0,    0,    0,    0,   30,    0,   30,   30,   30,
    0,   30,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   18,   18,    0,   18,   18,   18,   18,   18,    0,
   18,    0,    0,    0,    0,    0,   18,   18,    0,    0,
   18,   18,    0,    0,    0,    0,    0,    0,   50,   43,
    0,   27,    0,   48,   46,    0,   47,    0,   49,    0,
   27,   27,    0,   27,    0,    0,    0,   28,    0,    0,
    0,   44,    0,   45,    0,    0,   28,   28,    0,   28,
    0,    0,    0,    0,    0,   25,   25,   26,   26,    0,
    0,    0,    0,    0,   50,   43,    0,   24,   24,   48,
   46,    0,   47,    0,   49,   42,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   51,   44,    0,   45,
   40,    0,    0,   50,   43,    0,   32,   58,   48,   46,
    0,   47,   33,   49,   34,   35,   36,   37,   38,    0,
    0,    0,   39,    0,    0,    0,   44,    0,   45,   40,
    0,   42,   50,   43,    0,    0,    0,   48,   46,    0,
   47,    0,   49,    0,    0,    0,   50,   43,    0,    0,
    0,   48,   46,   85,   47,   44,   49,   45,   40,    0,
   42,   41,    0,    0,    0,    0,    0,    0,    0,   44,
    0,   45,   40,   50,   43,    0,    0,    0,   48,   46,
    0,   47,    0,   49,    0,    0,    0,   50,   43,   42,
   41,    0,   48,   46,    0,   47,   44,   49,   45,    0,
    0,   50,   43,   42,    0,    0,   48,   46,    0,   47,
   44,   49,   45,    0,    0,   50,    0,    0,    0,   41,
   48,   46,    0,   47,   44,   49,   45,    0,    0,   50,
   42,    0,    0,   41,   48,   46,    0,   47,   44,   49,
   45,    0,    0,    0,   42,    0,    0,    0,    0,    0,
   32,    0,   44,    0,   45,    0,   33,    0,   34,    0,
   41,   37,   38,    0,    0,    0,   39,    0,    0,    0,
    0,    0,    0,    0,   41,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   32,    0,    0,    0,
    0,    0,   33,    0,   34,   35,   36,   37,   38,    0,
    0,    0,   39,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   32,    0,    0,    0,    0,
    0,   33,    0,   34,   35,   36,   37,   38,    0,    0,
    0,   39,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   32,    0,    0,    0,    0,    0,
   33,    0,   34,   35,   36,   37,   38,    0,   32,    0,
   39,    0,    0,    0,   33,    0,   34,   35,   36,   37,
   38,    0,    0,    0,   39,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   32,    0,    0,    0,    0,
    0,   33,    0,   34,   35,    0,   37,   38,    0,   32,
    0,   39,    0,    0,    0,   33,    0,   34,    0,    0,
   37,   38,    0,   32,    0,   39,    0,    0,    0,   33,
    0,   34,    0,    0,   37,   38,    0,   32,    0,   39,
    0,    0,    0,   33,    0,   34,    0,    0,   37,   38,
    0,    0,    0,   39,    0,    0,    0,   33,    0,   34,
    0,    0,   37,   28,   29,   30,   31,   39,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   59,
   60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
   70,   71,   72,   73,   74,   75,   76,   77,    0,   78,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   88,    0,
    0,    0,   91,    0,   92,
};
static const short rh_check[] = {                        37,
    0,   40,  256,   37,   42,   43,  123,   45,   42,   47,
   40,   33,  266,   47,   37,  274,  266,  125,   40,   42,
   43,    0,   45,   45,   47,   41,   41,    0,   44,   44,
   41,   41,   41,   44,   44,   44,   41,   37,   38,   44,
    0,   41,   42,   43,   44,   45,    0,   47,   95,   26,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   59,
   60,   -1,   62,   63,   37,   38,   -1,   40,    0,   42,
   43,   -1,   45,   -1,   47,   -1,   -1,   -1,   38,   -1,
   -1,   41,   -1,   43,   44,   45,   59,   60,   -1,   62,
   63,   -1,   -1,   -1,   94,    0,   -1,   -1,   58,   59,
   60,   -1,   62,   63,  126,   -1,   38,   -1,    0,   41,
   -1,   43,   44,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   94,   -1,   -1,  124,  125,   58,   59,   60,   -1,
   62,   63,   -1,   38,   94,   -1,   41,    0,   -1,   44,
   -1,   -1,   -1,   -1,   -1,   -1,   38,    0,   -1,   41,
  123,  124,   44,   58,   59,   60,   -1,   62,   63,   -1,
   -1,   -1,   94,    0,  124,  125,   58,   59,   60,   -1,
   62,   63,   -1,   -1,   -1,   38,   -1,   -1,   41,   -1,
   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,   41,   94,
   -1,   44,  124,  125,   -1,   58,   59,   60,   -1,   62,
   63,   38,   94,   -1,   41,   58,   59,   44,   -1,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  124,
  125,   58,   59,   60,   -1,   62,   63,   -1,   -1,   -1,
   -1,   94,  124,  125,  256,  257,  258,   -1,  260,  261,
  262,  263,  264,   -1,  266,   -1,   -1,  270,   -1,   -1,
  272,  273,  275,   -1,  276,  277,   -1,   94,   -1,  259,
    0,  124,  125,   -1,   -1,  265,   -1,  267,  268,  269,
  270,  271,  125,   -1,   -1,  275,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  259,  124,  125,   -1,
    0,   -1,  265,   -1,  267,  268,  269,  270,  271,  259,
   -1,   41,  275,   -1,   44,  265,   -1,  267,  268,  269,
  270,  271,   -1,   -1,   -1,  275,   -1,   -1,   58,   59,
   -1,   -1,   -1,   63,   -1,   -1,   -1,  259,   38,   -1,
    0,   41,   -1,  265,   44,  267,  268,  269,  270,  271,
   -1,   -1,   -1,  275,   33,   -1,   -1,   -1,   58,   59,
   60,   40,   62,   63,  259,   -1,   45,   -1,   -1,    0,
  265,   -1,  267,  268,  269,  270,  271,  259,   38,   -1,
  275,   41,   -1,  265,   44,  267,  268,  269,  270,  271,
   -1,   -1,    0,  275,   94,  125,   -1,   33,   58,   59,
   60,   -1,   62,   63,   40,   -1,  259,   -1,    0,   45,
   41,   -1,  265,   44,  267,  268,  269,    0,  271,    0,
   -1,   -1,   -1,   -1,  124,  125,  269,   58,   59,    0,
   38,   -1,  259,   41,   94,   -1,   44,   -1,  265,   -1,
  267,  268,  269,   -1,  271,   -1,   38,  126,   -1,   41,
   58,   59,   44,   -1,   -1,   63,   -1,   38,   41,   -1,
   41,   44,   -1,   44,  124,  125,   58,   59,   -1,   -1,
   41,   63,   -1,   44,   -1,   58,   59,   58,   59,   -1,
   63,   -1,   63,   -1,   -1,   -1,   94,   58,   59,   -1,
  126,   -1,   63,   -1,  125,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   94,   -1,   37,   38,   -1,   -1,   -1,   42,
   43,   94,   45,   94,   47,   -1,  124,  125,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   59,   60,   -1,   62,
   63,   -1,  124,  125,   -1,   -1,   -1,   -1,  268,  269,
   -1,  124,  125,  124,  125,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  124,  125,   -1,   -1,   -1,   -1,  259,
   -1,   94,   -1,   -1,   -1,  265,   -1,  267,  268,  269,
   -1,  271,   -1,   -1,   -1,   -1,   -1,   -1,  257,  258,
   -1,  260,  261,  262,  263,  264,   -1,  266,   -1,   -1,
   -1,  124,  125,  272,  273,   -1,   -1,  276,  277,  259,
   -1,   -1,   -1,   -1,   -1,  265,   -1,  267,  268,  269,
   -1,  271,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  257,  258,   -1,  260,  261,  262,  263,  264,   -1,
  266,   -1,   -1,   -1,   -1,   -1,  272,  273,   -1,   -1,
  276,  277,   -1,   -1,   -1,   -1,   -1,   -1,   37,   38,
   -1,  259,   -1,   42,   43,   -1,   45,   -1,   47,   -1,
  268,  269,   -1,  271,   -1,   -1,   -1,  259,   -1,   -1,
   -1,   60,   -1,   62,   -1,   -1,  268,  269,   -1,  271,
   -1,   -1,   -1,   -1,   -1,  268,  269,  268,  269,   -1,
   -1,   -1,   -1,   -1,   37,   38,   -1,  268,  269,   42,
   43,   -1,   45,   -1,   47,   94,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   59,   60,   -1,   62,
   63,   -1,   -1,   37,   38,   -1,  259,   41,   42,   43,
   -1,   45,  265,   47,  267,  268,  269,  270,  271,   -1,
   -1,   -1,  275,   -1,   -1,   -1,   60,   -1,   62,   63,
   -1,   94,   37,   38,   -1,   -1,   -1,   42,   43,   -1,
   45,   -1,   47,   -1,   -1,   -1,   37,   38,   -1,   -1,
   -1,   42,   43,   58,   45,   60,   47,   62,   63,   -1,
   94,  124,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   60,
   -1,   62,   63,   37,   38,   -1,   -1,   -1,   42,   43,
   -1,   45,   -1,   47,   -1,   -1,   -1,   37,   38,   94,
  124,   -1,   42,   43,   -1,   45,   60,   47,   62,   -1,
   -1,   37,   38,   94,   -1,   -1,   42,   43,   -1,   45,
   60,   47,   62,   -1,   -1,   37,   -1,   -1,   -1,  124,
   42,   43,   -1,   45,   60,   47,   62,   -1,   -1,   37,
   94,   -1,   -1,  124,   42,   43,   -1,   45,   60,   47,
   62,   -1,   -1,   -1,   94,   -1,   -1,   -1,   -1,   -1,
  259,   -1,   60,   -1,   62,   -1,  265,   -1,  267,   -1,
  124,  270,  271,   -1,   -1,   -1,  275,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  124,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  259,   -1,   -1,   -1,
   -1,   -1,  265,   -1,  267,  268,  269,  270,  271,   -1,
   -1,   -1,  275,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  259,   -1,   -1,   -1,   -1,
   -1,  265,   -1,  267,  268,  269,  270,  271,   -1,   -1,
   -1,  275,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  259,   -1,   -1,   -1,   -1,   -1,
  265,   -1,  267,  268,  269,  270,  271,   -1,  259,   -1,
  275,   -1,   -1,   -1,  265,   -1,  267,  268,  269,  270,
  271,   -1,   -1,   -1,  275,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  259,   -1,   -1,   -1,   -1,
   -1,  265,   -1,  267,  268,   -1,  270,  271,   -1,  259,
   -1,  275,   -1,   -1,   -1,  265,   -1,  267,   -1,   -1,
  270,  271,   -1,  259,   -1,  275,   -1,   -1,   -1,  265,
   -1,  267,   -1,   -1,  270,  271,   -1,  259,   -1,  275,
   -1,   -1,   -1,  265,   -1,  267,   -1,   -1,  270,  271,
   -1,   -1,   -1,  275,   -1,   -1,   -1,  265,   -1,  267,
   -1,   -1,  270,   16,   17,   18,   19,  275,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   32,
   33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
   43,   44,   45,   46,   47,   48,   49,   50,   -1,   52,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   81,   -1,
   -1,   -1,   85,   -1,   87,
};
#define YYFINAL 1
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 278
#if YYDEBUG
static const char *yyname[] = {

"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
"'!'",0,0,0,"'%'","'&'",0,"'('","')'","'*'","'+'","','","'-'",0,"'/'",0,0,0,0,0,
0,0,0,0,0,"':'","';'","'<'",0,"'>'","'?'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,"'^'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,"'{'","'|'","'}'","'~'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"BLTIN","DATESPEC","EQ","FIELD",
"FILENAME_PATTERN","FILETIME_FIELD","FSTYPE","FUNCTION","GE","IDENTIFIER","LE",
"LOGAND","LOGOR","LSHIFT","NE","NUMBER","PARAM","RETURN","RSHIFT","SYSTEM_CALL",
"TIME_FIELD","UNIMINUS",
};
static const char *yyrule[] = {
"$accept : program",
"program : functions",
"program : functions expression",
"program : functions expression ';'",
"functions :",
"functions : functions function",
"$$1 :",
"function : IDENTIFIER $$1 function_body",
"$$2 :",
"$$3 :",
"function : IDENTIFIER $$2 '(' idlist ')' $$3 function_body",
"function : error",
"idlist :",
"idlist : IDENTIFIER",
"idlist : idlist ',' IDENTIFIER",
"idlist : error",
"function_body : '{' return expression ';' '}'",
"function_body : '{' return expression '}'",
"return :",
"return : RETURN",
"expression : factor",
"expression : expression '?' expression ':' expression",
"expression : expression LOGOR expression",
"expression : expression LOGAND expression",
"expression : expression '|' expression",
"expression : expression '^' expression",
"expression : expression '&' expression",
"expression : expression EQ expression",
"expression : expression NE expression",
"expression : expression '<' expression",
"expression : expression '>' expression",
"expression : expression LE expression",
"expression : expression GE expression",
"expression : expression RSHIFT expression",
"expression : expression LSHIFT expression",
"expression : expression '+' expression",
"expression : expression '-' expression",
"expression : expression '*' expression",
"expression : expression '/' expression",
"expression : expression '%' expression",
"expression : '~' expression",
"expression : '!' expression",
"expression : '-' expression",
"$$4 :",
"factor : FUNCTION $$4 paramlist",
"$$5 :",
"factor : SYSTEM_CALL $$5 paramlist",
"factor : '(' expression ')'",
"factor : NUMBER",
"factor : FILENAME_PATTERN",
"factor : FSTYPE",
"factor : BLTIN",
"factor : FIELD",
"factor : PARAM",
"factor : IDENTIFIER",
"factor : DATESPEC",
"factor : FILETIME_FIELD",
"factor : TIME_FIELD",
"paramlist :",
"paramlist : '(' exprlist ')'",
"exprlist :",
"exprlist : expression",
"exprlist : exprlist ',' expression",

};
#endif

int      yydebug;
int      yynerrs;

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 500
#define YYMAXDEPTH  500
#endif
#endif

#define YYINITSTACKSIZE 500

typedef struct {
    unsigned stacksize;
    short    *s_base;
    short    *s_mark;
    short    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
} YYSTACKDATA;
/* variables for the parser stack */
static YYSTACKDATA yystack;
#line 726 "rhgram.y"

void rh_error(const char *msg)
{
    extern const int		yyleng;
    extern const char * const	yytext;

#if defined(DEBUG2)
    (void) fprintf(stderr, "yyleng = %d, yytext = '%s'\n", yyleng, yytext);
#endif

    if (yyleng) {
	if (Line_num < 0) {
	    (void) fprintf(stderr, "%s: %s: %d: %s\n",
			   Program_name, Input_name, -Line_num, msg);
	}
	else {
	    (void) fprintf(stderr, "%s: %s: %d: %s at '%s'\n",
			   Program_name, Input_name, Line_num, msg, yytext);
	}
	(void) fprintf(stderr, " Line: %s\nError: %*s\n",
		       Line, Line_pos - (yyleng - 1), "^");
    }
    else {
	(void) fprintf(stderr, "%s: %s: %s\n", Program_name, Input_name, msg);
	(void) fprintf(stderr, "%s: unexpected end of file after line %d\n",
		       Program_name, Line_num);
	(void) fprintf(stderr, "Last Line: %s\n", Line);
    }
    exit(1);
    /*NOTREACHED*/
}

static node *make_node(void	      (*func)(),
		       instr_value *	value)
{
    node *	new_node;

    new_node = ALLOCATE(sizeof(node));
    new_node->code.func = func;
    new_node->code.value = *value;
    new_node->left = new_node->right = (node *) NULL;
    new_node->middle = (node *) NULL;
    new_node->count = -1;
    new_node->trees = (node **) NULL;

    return new_node;
}

node *copy_tree(node *tree)
{
    node *	new_tree;


    if (tree == (node *) NULL) {
	return tree;
    }

    new_tree = ALLOCATE(sizeof(node));

    new_tree->code.func = tree->code.func;
    if (new_tree->code.func == c_date) {
	new_tree->code.value.di = ALLOCATE(sizeof(date_info));
	*(new_tree->code.value.di) = *tree->code.value.di;
    }
    else if (new_tree->code.func == c_pattern
	     || new_tree->code.func == c_fncase
	     || new_tree->code.func == c_fnigncase) {
	new_tree->code.value.pattern = SAVESTR(tree->code.value.pattern);
    }
    else {
	new_tree->code.value = tree->code.value;
    }
    
    new_tree->left = copy_tree(tree->left);
    new_tree->right = copy_tree(tree->right);
    new_tree->middle = copy_tree(tree->middle);
    
    new_tree->count = tree->count;
    if (new_tree->count <= 0) {
	new_tree->trees = (node **) NULL;
    }
    else {
	int	i;
	
	new_tree->trees = ALLOCATE(sizeof(node *) * new_tree->count);
	for (i = 0; i < new_tree->count; i++) {
	    new_tree->trees[i] = copy_tree(tree->trees[i]);
	}
    }
    
    return new_tree;
}

void free_tree(node *tree)
{
    if (tree != (node *) NULL) {
	free_tree(tree->left);
	free_tree(tree->right);
	free_tree(tree->middle);
	if (tree->code.func == c_date) {
	    free((char *) tree->code.value.di);
	}
	else if (tree->code.func == c_fstype) {
	    free(tree->code.value.fstype);
	}
	else if (tree->code.func == c_pattern
		 || tree->code.func == c_fncase
		 || tree->code.func == c_fnigncase) {
	    free(tree->code.value.pattern);
	}
	else if (tree->code.func == UNRES_FUNC && tree->count > 0) {
	    int i;
	    
	    for (i = 0; i < tree->count; i++) {
		free_tree(tree->trees[i]);
	    }
	    free((char *) tree->trees);
	}
	free((char *) tree);
    }

    return;
}

static int pattern(const char *p)
{
    for (/*void*/ ; *p != '\0' ; ++p) {
	if (*p == '\\') {
	    if (*++p == '\0') {
		break;
	    }
	}
	else if (*p == '?' || *p == '*' || *p == '[') {
	    return 1;
	}
    }
    return 0;
}
#line 765 "y.tab.c"

#if YYDEBUG
#include <stdio.h>		/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = data->s_mark - data->s_base;
    newss = (short *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return -1;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return -1;

    data->l_base = newvs;
    data->l_mark = newvs + i;

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab

int
YYPARSE_DECL()
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack)) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
        {
            goto yyoverflow;
        }
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;

    yyerror("syntax error");

    goto yyerrlab;

yyerrlab:
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yystack.s_mark]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
                {
                    goto yyoverflow;
                }
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
                --yystack.s_mark;
                --yystack.l_mark;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
    switch (yyn)
    {
case 1:
#line 96 "rhgram.y"
	{
			    rh_lex_finish();
			}
break;
case 2:
#line 100 "rhgram.y"
	{
			    rh_lex_finish();
			    if (Start_expr == (node *) NULL) {
				Start_expr = yystack.l_mark[0].tree;
			    }
			    else {
				error("too many start expressions specified");
			    }
			}
break;
case 3:
#line 110 "rhgram.y"
	{
			    rh_lex_finish();
			    if (Start_expr == (node *) NULL) {
				Start_expr = yystack.l_mark[-1].tree;
			    }
			    else {
				error("too many start expressions specified");
			    }
			}
break;
case 6:
#line 126 "rhgram.y"
	{
			    yystack.l_mark[0].sym->type = FUNCTION;
			    yystack.l_mark[0].sym->func = c_func;
			    yystack.l_mark[0].sym->value = -1;
			    yystack.l_mark[0].sym->tree = (node *) NULL;
			}
break;
case 7:
#line 133 "rhgram.y"
	{
			    yystack.l_mark[-2].sym->tree = yystack.l_mark[0].tree;
			    yystack.l_mark[-2].sym->num_params = 0;
			}
break;
case 8:
#line 138 "rhgram.y"
	{
			    yystack.l_mark[0].sym->type = FUNCTION;
			    yystack.l_mark[0].sym->func = c_func;
			    yystack.l_mark[0].sym->value = -1;
			    yystack.l_mark[0].sym->tree = (node *) NULL;
			}
break;
case 9:
#line 145 "rhgram.y"
	{
			    yystack.l_mark[-4].sym->num_params = yystack.l_mark[-1].value;
			}
break;
case 10:
#line 149 "rhgram.y"
	{
			    symbol *sym;

			    yystack.l_mark[-6].sym->tree = yystack.l_mark[0].tree;

			    while (Symbol_table->type == PARAM) {
				if (Symbol_table->ref_count == 0
				    && Issue_warnings) {
				    warning("warning: function '%s' parameter '%s' is not referenced",
					    yystack.l_mark[-6].sym->name, Symbol_table->name);
				}
				sym = Symbol_table;
				Symbol_table = Symbol_table->next;
				free(sym->name);
				free((char *) sym);
			    }
			}
break;
case 11:
#line 167 "rhgram.y"
	{
			    rh_error("expected function definition");
			}
break;
case 12:
#line 173 "rhgram.y"
	{
			    yyval.value = 0;
			}
break;
case 13:
#line 177 "rhgram.y"
	{
			    yystack.l_mark[0].sym->type = PARAM;
			    yystack.l_mark[0].sym->func = c_param;
			    yystack.l_mark[0].sym->value = 0;
			    yystack.l_mark[0].sym->ref_count = 0;
			    yyval.value = 1;
			}
break;
case 14:
#line 185 "rhgram.y"
	{
			    yystack.l_mark[0].sym->type = PARAM;
			    yystack.l_mark[0].sym->func = c_param;
			    yystack.l_mark[0].sym->value = yystack.l_mark[-2].value;
			    yystack.l_mark[0].sym->ref_count = 0;
			    yyval.value = yystack.l_mark[-2].value + 1;
			}
break;
case 15:
#line 193 "rhgram.y"
	{
			    rh_error("expected identifier");
			}
break;
case 16:
#line 199 "rhgram.y"
	{ yyval.tree = yystack.l_mark[-2].tree; }
break;
case 17:
#line 201 "rhgram.y"
	{ yyval.tree = yystack.l_mark[-1].tree; }
break;
case 20:
#line 209 "rhgram.y"
	{ yyval.tree = yystack.l_mark[0].tree; }
break;
case 21:
#line 211 "rhgram.y"
	{
			    if (yystack.l_mark[-4].tree->code.func == c_number
				&& yystack.l_mark[-2].tree->code.func == c_number
				&& yystack.l_mark[0].tree->code.func == c_number) {
				yyval.tree = yystack.l_mark[-4].tree;
				yyval.tree->code.value.l =
				    yystack.l_mark[-4].tree->code.value.l ? yystack.l_mark[-2].tree->code.value.l : yystack.l_mark[0].tree->code.value.l;
				free((char *) yystack.l_mark[-2].tree);
				free((char *) yystack.l_mark[0].tree);
			    }
			    else {
				value.l = 0L;
				yyval.tree = make_node(TERTIARY, &value);
				yyval.tree->left = yystack.l_mark[-4].tree;
				yyval.tree->middle = yystack.l_mark[-2].tree;
				yyval.tree->right = yystack.l_mark[0].tree;
			    }
			}
break;
case 22:
#line 230 "rhgram.y"
	{
			    if (yystack.l_mark[-2].tree->code.func == c_number
				&& yystack.l_mark[0].tree->code.func == c_number) {
				yyval.tree = yystack.l_mark[-2].tree;
				yyval.tree->code.value.l =
				    yystack.l_mark[-2].tree->code.value.l || yystack.l_mark[0].tree->code.value.l;
				free((char *) yystack.l_mark[0].tree);
			    }
			    else {
				value.l = 0L;
				yyval.tree = make_node(c_or, &value);
				yyval.tree->left = yystack.l_mark[-2].tree;
				yyval.tree->right = yystack.l_mark[0].tree;
			    }
			}
break;
case 23:
#line 246 "rhgram.y"
	{
			    if (yystack.l_mark[-2].tree->code.func == c_number
				&& yystack.l_mark[0].tree->code.func == c_number) {
				yyval.tree = yystack.l_mark[-2].tree;
				yyval.tree->code.value.l =
				    yystack.l_mark[-2].tree->code.value.l && yystack.l_mark[0].tree->code.value.l;
				free((char *) yystack.l_mark[0].tree);
			    }
			    else {
				value.l = 0L;
				yyval.tree = make_node(c_and, &value);
				yyval.tree->left = yystack.l_mark[-2].tree;
				yyval.tree->right = yystack.l_mark[0].tree;
			    }
			}
break;
case 24:
#line 262 "rhgram.y"
	{
			    if (yystack.l_mark[-2].tree->code.func == c_number
				&& yystack.l_mark[0].tree->code.func == c_number) {
				yyval.tree = yystack.l_mark[-2].tree;
				yyval.tree->code.value.l =
				    yystack.l_mark[-2].tree->code.value.l | yystack.l_mark[0].tree->code.value.l;
				free((char *) yystack.l_mark[0].tree);
			    }
			    else {
				value.l = 0L;
				yyval.tree = make_node(c_bor, &value);
				yyval.tree->left = yystack.l_mark[-2].tree;
				yyval.tree->right = yystack.l_mark[0].tree;
			    }
			}
break;
case 25:
#line 278 "rhgram.y"
	{
			    if (yystack.l_mark[-2].tree->code.func == c_number
				&& yystack.l_mark[0].tree->code.func == c_number) {
				yyval.tree = yystack.l_mark[-2].tree;
				yyval.tree->code.value.l =
				    yystack.l_mark[-2].tree->code.value.l ^ yystack.l_mark[0].tree->code.value.l;
				free((char *) yystack.l_mark[0].tree);
			    }
			    else {
				value.l = 0L;
				yyval.tree = make_node(c_bxor, &value);
				yyval.tree->left = yystack.l_mark[-2].tree;
				yyval.tree->right = yystack.l_mark[0].tree;
			    }
			}
break;
case 26:
#line 294 "rhgram.y"
	{
			    if (yystack.l_mark[-2].tree->code.func == c_number
				&& yystack.l_mark[0].tree->code.func == c_number) {
				yyval.tree = yystack.l_mark[-2].tree;
				yyval.tree->code.value.l =
				    yystack.l_mark[-2].tree->code.value.l & yystack.l_mark[0].tree->code.value.l;
				free((char *) yystack.l_mark[0].tree);
			    }
			    else {
				value.l = 0L;
				yyval.tree = make_node(c_band, &value);
				yyval.tree->left = yystack.l_mark[-2].tree;
				yyval.tree->right = yystack.l_mark[0].tree;
			    }
			}
break;
case 27:
#line 310 "rhgram.y"
	{
			    if (yystack.l_mark[-2].tree->code.func == c_number
				&& yystack.l_mark[0].tree->code.func == c_number) {
				yyval.tree = yystack.l_mark[-2].tree;
				yyval.tree->code.value.l =
				    yystack.l_mark[-2].tree->code.value.l == yystack.l_mark[0].tree->code.value.l;
				free((char *) yystack.l_mark[0].tree);
			    }
			    else {
				value.l = 0L;
				yyval.tree = make_node(c_eq, &value);
				yyval.tree->left = yystack.l_mark[-2].tree;
				yyval.tree->right = yystack.l_mark[0].tree;
			    }
			}
break;
case 28:
#line 326 "rhgram.y"
	{
			    if (yystack.l_mark[-2].tree->code.func == c_number
				&& yystack.l_mark[0].tree->code.func == c_number) {
				yyval.tree = yystack.l_mark[-2].tree;
				yyval.tree->code.value.l =
				    yystack.l_mark[-2].tree->code.value.l != yystack.l_mark[0].tree->code.value.l;
				free((char *) yystack.l_mark[0].tree);
			    }
			    else {
				value.l = 0L;
				yyval.tree = make_node(c_ne, &value);
				yyval.tree->left = yystack.l_mark[-2].tree;
				yyval.tree->right = yystack.l_mark[0].tree;
			    }
			}
break;
case 29:
#line 342 "rhgram.y"
	{
			    if (yystack.l_mark[-2].tree->code.func == c_number
				&& yystack.l_mark[0].tree->code.func == c_number) {
				yyval.tree = yystack.l_mark[-2].tree;
				yyval.tree->code.value.l =
				    yystack.l_mark[-2].tree->code.value.l < yystack.l_mark[0].tree->code.value.l;
				free((char *) yystack.l_mark[0].tree);
			    }
			    else {
				value.l = 0L;
				yyval.tree = make_node(c_lt, &value);
				yyval.tree->left = yystack.l_mark[-2].tree;
				yyval.tree->right = yystack.l_mark[0].tree;
			    }
			}
break;
case 30:
#line 358 "rhgram.y"
	{
			    if (yystack.l_mark[-2].tree->code.func == c_number
				&& yystack.l_mark[0].tree->code.func == c_number) {
				yyval.tree = yystack.l_mark[-2].tree;
				yyval.tree->code.value.l =
				    yystack.l_mark[-2].tree->code.value.l > yystack.l_mark[0].tree->code.value.l;
				free((char *) yystack.l_mark[0].tree);
			    }
			    else {
				value.l = 0L;
				yyval.tree = make_node(c_gt, &value);
				yyval.tree->left = yystack.l_mark[-2].tree;
				yyval.tree->right = yystack.l_mark[0].tree;
			    }
			}
break;
case 31:
#line 374 "rhgram.y"
	{
			    if (yystack.l_mark[-2].tree->code.func == c_number
				&& yystack.l_mark[0].tree->code.func == c_number) {
				yyval.tree = yystack.l_mark[-2].tree;
				yyval.tree->code.value.l =
				    yystack.l_mark[-2].tree->code.value.l <= yystack.l_mark[0].tree->code.value.l;
				free((char *) yystack.l_mark[0].tree);
			    }
			    else {
				value.l = 0L;
				yyval.tree = make_node(c_le, &value);
				yyval.tree->left = yystack.l_mark[-2].tree;
				yyval.tree->right = yystack.l_mark[0].tree;
			    }
			}
break;
case 32:
#line 390 "rhgram.y"
	{
			    if (yystack.l_mark[-2].tree->code.func == c_number
				&& yystack.l_mark[0].tree->code.func == c_number) {
				yyval.tree = yystack.l_mark[-2].tree;
				yyval.tree->code.value.l =
				    yystack.l_mark[-2].tree->code.value.l >= yystack.l_mark[0].tree->code.value.l;
				free((char *) yystack.l_mark[0].tree);
			    }
			    else {
				value.l = 0L;
				yyval.tree = make_node(c_ge, &value);
				yyval.tree->left = yystack.l_mark[-2].tree;
				yyval.tree->right = yystack.l_mark[0].tree;
			    }
			}
break;
case 33:
#line 406 "rhgram.y"
	{
			    if (yystack.l_mark[-2].tree->code.func == c_number
				&& yystack.l_mark[0].tree->code.func == c_number) {
				yyval.tree = yystack.l_mark[-2].tree;
				yyval.tree->code.value.l =
				    yystack.l_mark[-2].tree->code.value.l >> yystack.l_mark[0].tree->code.value.l;
				free((char *) yystack.l_mark[0].tree);
			    }
			    else {
				value.l = 0L;
				yyval.tree = make_node(c_rshift, &value);
				yyval.tree->left = yystack.l_mark[-2].tree;
				yyval.tree->right = yystack.l_mark[0].tree;
			    }
			}
break;
case 34:
#line 422 "rhgram.y"
	{
			    if (yystack.l_mark[-2].tree->code.func == c_number
				&& yystack.l_mark[0].tree->code.func == c_number) {
				yyval.tree = yystack.l_mark[-2].tree;
				yyval.tree->code.value.l =
				    yystack.l_mark[-2].tree->code.value.l << yystack.l_mark[0].tree->code.value.l;
				free((char *) yystack.l_mark[0].tree);
			    }
			    else {
				value.l = 0L; 
				yyval.tree = make_node(c_lshift, &value);
				yyval.tree->left = yystack.l_mark[-2].tree;
				yyval.tree->right = yystack.l_mark[0].tree;
			    }
			}
break;
case 35:
#line 438 "rhgram.y"
	{
			    if (yystack.l_mark[-2].tree->code.func == c_number
				&& yystack.l_mark[0].tree->code.func == c_number) {
				yyval.tree = yystack.l_mark[-2].tree;
				yyval.tree->code.value.l =
				    yystack.l_mark[-2].tree->code.value.l + yystack.l_mark[0].tree->code.value.l;
				free((char *) yystack.l_mark[0].tree);
			    }
			    else {
				value.l = 0L;
				yyval.tree = make_node(c_plus, &value);
				yyval.tree->left = yystack.l_mark[-2].tree;
				yyval.tree->right = yystack.l_mark[0].tree;
			    }
			}
break;
case 36:
#line 454 "rhgram.y"
	{
			    if (yystack.l_mark[-2].tree->code.func == c_number
				&& yystack.l_mark[0].tree->code.func == c_number) {
				yyval.tree = yystack.l_mark[-2].tree;
				yyval.tree->code.value.l =
				    yystack.l_mark[-2].tree->code.value.l - yystack.l_mark[0].tree->code.value.l;
				free((char *) yystack.l_mark[0].tree);
			    }
			    else {
				value.l = 0L;
				yyval.tree = make_node(c_minus, &value);
				yyval.tree->left = yystack.l_mark[-2].tree;
				yyval.tree->right = yystack.l_mark[0].tree;
			    }
			}
break;
case 37:
#line 470 "rhgram.y"
	{
			    if (yystack.l_mark[-2].tree->code.func == c_number
				&& yystack.l_mark[0].tree->code.func == c_number) {
				yyval.tree = yystack.l_mark[-2].tree;
				yyval.tree->code.value.l =
				    yystack.l_mark[-2].tree->code.value.l * yystack.l_mark[0].tree->code.value.l;
				free((char *) yystack.l_mark[0].tree);
			    }
			    else {
				value.l = 0L;
				yyval.tree = make_node(c_mul, &value);
				yyval.tree->left = yystack.l_mark[-2].tree;
				yyval.tree->right = yystack.l_mark[0].tree;
			    }
			}
break;
case 38:
#line 486 "rhgram.y"
	{
			    if (yystack.l_mark[-2].tree->code.func == c_number
				&& yystack.l_mark[0].tree->code.func == c_number) {
				yyval.tree = yystack.l_mark[-2].tree;
				yyval.tree->code.value.l =
				    yystack.l_mark[-2].tree->code.value.l / yystack.l_mark[0].tree->code.value.l;
				free((char *) yystack.l_mark[0].tree);
			    }
			    else {
				value.l = 0L;
				yyval.tree = make_node(c_div, &value);
				yyval.tree->left = yystack.l_mark[-2].tree;
				yyval.tree->right = yystack.l_mark[0].tree;
			    }
			}
break;
case 39:
#line 502 "rhgram.y"
	{
			    if (yystack.l_mark[-2].tree->code.func == c_number
				&& yystack.l_mark[0].tree->code.func == c_number) {
				yyval.tree = yystack.l_mark[-2].tree;
				yyval.tree->code.value.l =
				    yystack.l_mark[-2].tree->code.value.l % yystack.l_mark[0].tree->code.value.l;
				free((char *) yystack.l_mark[0].tree);
			    }
			    else {
				value.l = 0L;
				yyval.tree = make_node(c_mod, &value);
				yyval.tree->left = yystack.l_mark[-2].tree;
				yyval.tree->right = yystack.l_mark[0].tree;
			    }
			}
break;
case 40:
#line 518 "rhgram.y"
	{
			    if (yystack.l_mark[0].tree->code.func == c_number) {
				yyval.tree = yystack.l_mark[0].tree;
				yyval.tree->code.value.l = ~ yyval.tree->code.value.l;
			    }
			    else {
				value.l = 0L;
				yyval.tree = make_node(c_bnot, &value);
				yyval.tree->right = yystack.l_mark[0].tree;
			    }
			}
break;
case 41:
#line 530 "rhgram.y"
	{
			    if (yystack.l_mark[0].tree->code.func == c_number) {
				yyval.tree = yystack.l_mark[0].tree;
				yyval.tree->code.value.l = ! yyval.tree->code.value.l;
			    }
			    else {
				value.l = 0L;
				yyval.tree = make_node(c_not, &value);
				yyval.tree->right = yystack.l_mark[0].tree;
			    }
			}
break;
case 42:
#line 542 "rhgram.y"
	{
			    if (yystack.l_mark[0].tree->code.func == c_number) {
				yyval.tree = yystack.l_mark[0].tree;
				yyval.tree->code.value.l = - yyval.tree->code.value.l;
			    }
			    else {
				value.l = 0L;
				yyval.tree = make_node(c_uniminus, &value);
				yyval.tree->right = yystack.l_mark[0].tree;
			    }
			}
break;
case 43:
#line 556 "rhgram.y"
	{
			    yyval.line_info.line = SAVESTR(Line);
			    yyval.line_info.line_num = Line_num;
			    yyval.line_info.line_pos = Line_pos - strlen(yystack.l_mark[0].sym->name);
			}
break;
case 44:
#line 562 "rhgram.y"
	{
			    if (yystack.l_mark[-2].sym->num_params != yystack.l_mark[0].tree->count) {
				free(Line);
				Line = yystack.l_mark[-1].line_info.line;
				Line_num = -(yystack.l_mark[-1].line_info.line_num);
				Line_pos = yystack.l_mark[-1].line_info.line_pos;
				rh_error("function called with wrong number of arguments");
			    }
			    if (yystack.l_mark[-2].sym->tree != (node *) NULL
				&& yystack.l_mark[-2].sym->tree->left == (node *) NULL
				&& yystack.l_mark[-2].sym->tree->right == (node *) NULL) {
				int i;
				
				if (yystack.l_mark[-2].sym->tree->code.func == c_param) {
				    for (i = 0; i < yystack.l_mark[0].tree->count; i++) {
					if (i == yystack.l_mark[-2].sym->tree->code.value.param) {
					    yyval.tree = yystack.l_mark[0].tree->trees[i];
					}
					else {
					    free_tree(yystack.l_mark[0].tree->trees[i]);
					}
				    }
				}
				else {
				    yyval.tree = copy_tree(yystack.l_mark[-2].sym->tree);
				    for (i = 0; i < yystack.l_mark[0].tree->count; i++) {
					free_tree(yystack.l_mark[0].tree->trees[i]);
				    }
				}
				if (yystack.l_mark[0].tree->trees != (node **) NULL) {
				    free((char *) yystack.l_mark[0].tree->trees);
				}
				free((char *) yystack.l_mark[0].tree);
			    }
			    else {
				yyval.tree = yystack.l_mark[0].tree;
				yyval.tree->code.func = UNRES_FUNC;
				yyval.tree->code.value.func_name = yystack.l_mark[-2].sym->name;
			    }
			    free(yystack.l_mark[-1].line_info.line);
			}
break;
case 45:
#line 604 "rhgram.y"
	{
			    yyval.line_info.line = SAVESTR(Line);
			    yyval.line_info.line_num = Line_num;
			    yyval.line_info.line_pos = Line_pos - strlen(yystack.l_mark[0].sym->name);
			}
break;
case 46:
#line 610 "rhgram.y"
	{
			    if (yystack.l_mark[-2].sym->num_params != yystack.l_mark[0].tree->count) {
				free(Line);
				Line = yystack.l_mark[-1].line_info.line;
				Line_num = -(yystack.l_mark[-1].line_info.line_num);
				Line_pos = yystack.l_mark[-1].line_info.line_pos;
				rh_error("function called with wrong number of arguments");
			    }
			    yyval.tree = yystack.l_mark[0].tree;
			    yyval.tree->code.func = c_syscall;
			    yyval.tree->code.value.sym = yystack.l_mark[-2].sym;
			    free(yystack.l_mark[-1].line_info.line);
			}
break;
case 47:
#line 624 "rhgram.y"
	{ yyval.tree = yystack.l_mark[-1].tree; }
break;
case 48:
#line 626 "rhgram.y"
	{
			    value.l = yystack.l_mark[0].value;
			    yyval.tree = make_node(c_number, &value);
			}
break;
case 49:
#line 631 "rhgram.y"
	{
			    if (attr.dot_special && strcmp(yystack.l_mark[0].str, "*") == 0) {
				value.l = 1L;
				yyval.tree = make_node(c_number, &value);
			    }
			    else {
				value.pattern = yystack.l_mark[0].str;
				if (pattern(yystack.l_mark[0].str)) {
				    yyval.tree = make_node(c_pattern, &value);
				}
				else if (attr.ignore_case) {
				    yyval.tree = make_node(c_fnigncase, &value);
				}
				else {
				    yyval.tree = make_node(c_fncase, &value);
				}
			    }
			}
break;
case 50:
#line 650 "rhgram.y"
	{
			    value.fstype = yystack.l_mark[0].str;
			    yyval.tree = make_node(c_fstype, &value);
			}
break;
case 51:
#line 655 "rhgram.y"
	{
			    value.l = yystack.l_mark[0].sym->value;
			    yyval.tree = make_node(yystack.l_mark[0].sym->func, &value);
			}
break;
case 52:
#line 660 "rhgram.y"
	{
			    value.l = yystack.l_mark[0].sym->value;
			    yyval.tree = make_node(yystack.l_mark[0].sym->func, &value);
			}
break;
case 53:
#line 665 "rhgram.y"
	{
			    value.param = yystack.l_mark[0].sym->value;
			    yyval.tree = make_node(c_param, &value);
			    yystack.l_mark[0].sym->ref_count++;
			}
break;
case 54:
#line 671 "rhgram.y"
	{ rh_error("undefined identifier"); }
break;
case 55:
#line 673 "rhgram.y"
	{
			    value.di = yystack.l_mark[0].di;
			    yyval.tree = make_node(c_date, &value);
			}
break;
case 56:
#line 678 "rhgram.y"
	{
			    value.di = yystack.l_mark[0].di;
			    yyval.tree = make_node(c_date, &value);
			}
break;
case 57:
#line 683 "rhgram.y"
	{
			    value.l = yystack.l_mark[0].sym->value;
			    yyval.tree = make_node(yystack.l_mark[0].sym->func, &value);
			}
break;
case 58:
#line 690 "rhgram.y"
	{
			    value.l = 0L;
			    yyval.tree = make_node(EXPR_LIST, &value);
			    yyval.tree->count = 0;
			    yyval.tree->trees = (node **) NULL;
			}
break;
case 59:
#line 697 "rhgram.y"
	{ yyval.tree = yystack.l_mark[-1].tree; }
break;
case 60:
#line 700 "rhgram.y"
	{
			    value.l = 0L;
			    yyval.tree = make_node(EXPR_LIST, &value);
			    yyval.tree->count = 0;
			    yyval.tree->trees = (node **) NULL;
			}
break;
case 61:
#line 707 "rhgram.y"
	{
			    value.l = 0L;
			    yyval.tree = make_node(EXPR_LIST, &value);
			    yyval.tree->count = 1;
			    yyval.tree->trees = ALLOCATE(sizeof(node *));
			    yyval.tree->trees[0] = yystack.l_mark[0].tree;
			}
break;
case 62:
#line 715 "rhgram.y"
	{
			    yyval.tree = yystack.l_mark[-2].tree;
			    yyval.tree->count++;
			    yyval.tree->trees =
				REALLOCATE(yyval.tree->trees,
					   (sizeof(node *)) * yyval.tree->count);
			    yyval.tree->trees[yyval.tree->count - 1] = yystack.l_mark[0].tree;
			}
break;
#line 1694 "y.tab.c"
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
        if (yychar < 0)
        {
            if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yystack.s_mark, yystate);
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
    {
        goto yyoverflow;
    }
    *++yystack.s_mark = (short) yystate;
    *++yystack.l_mark = yyval;
    goto yyloop;

yyoverflow:
    yyerror("yacc stack overflow");

yyabort:
    yyfreestack(&yystack);
    return (1);

yyaccept:
    yyfreestack(&yystack);
    return (0);
}

#ifndef lint
static const char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif

#include <stdlib.h>
#include <string.h>

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20070509

#define YYEMPTY (-1)
#define yyclearin    (yychar = YYEMPTY)
#define yyerrok      (yyerrflag = 0)
#define YYRECOVERING (yyerrflag != 0)

extern int yyparse(void);

static int yygrowstack(void);
#define yyparse gd_parse
#define yylex gd_lex
#define yyerror gd_error
#define yychar gd_char
#define yyval gd_val
#define yylval gd_lval
#define yydebug gd_debug
#define yynerrs gd_nerrs
#define yyerrflag gd_errflag
#define yyss gd_ss
#define yyssp gd_ssp
#define yyvs gd_vs
#define yyvsp gd_vsp
#define yylhs gd_lhs
#define yylen gd_len
#define yydefred gd_defred
#define yydgoto gd_dgoto
#define yysindex gd_sindex
#define yyrindex gd_rindex
#define yygindex gd_gindex
#define yytable gd_table
#define yycheck gd_check
#define yyname gd_name
#define yyrule gd_rule
#define YYPREFIX "gd_"
#line 3 "getdate.y"
	/* getdate.y						*/
	/*							*/
	/* 	Steven M. Bellovin (unc!smb)			*/
	/*	Dept. of Computer Science			*/
	/*	University of North Carolina at Chapel Hill	*/
	/*	@(#)getdate.y	2.13	9/16/86			*/

#if !defined(lint)
static char rcsid[] = "$Id: getdate.y,v 1.1 1994/01/12 19:35:54 rick Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include "getdate.h"

#define daysec (24L*60L*60L)

static int timeflag, zoneflag, dateflag, dayflag, relflag;
static time_t relsec, relmonth;
static int hh, mm, ss, merid, dylight;
static int dayord, dayreq;
static int month, day, year;
static int ourzone;

#define AM 1
#define PM 2
#define DAYLIGHT 1
#define STANDARD 2
#define MAYBE    3

static int gd_lex(), lookup();
static time_t timeconv(), daylcorr(), dateconv(), daylcorr();

#line 89 "y.tab.c"
#define ID 257
#define MONTH 258
#define DAY 259
#define MERIDIAN 260
#define NUMBER 261
#define UNIT 262
#define MUNIT 263
#define SUNIT 264
#define ZONE 265
#define DAYZONE 266
#define AGO 267
#define YYERRCODE 256
short gd_lhs[] = {                                        -1,
    0,    0,    1,    1,    1,    1,    1,    1,    7,    2,
    2,    2,    2,    2,    2,    2,    3,    3,    5,    5,
    5,    4,    4,    4,    4,    4,    4,    6,    6,    6,
    6,    6,    6,    6,
};
short gd_len[] = {                                         2,
    0,    2,    1,    1,    1,    1,    1,    1,    1,    2,
    3,    4,    4,    5,    6,    6,    1,    1,    1,    2,
    2,    3,    5,    2,    4,    2,    3,    2,    2,    2,
    1,    1,    1,    2,
};
short gd_defred[] = {                                      1,
    0,    0,    0,    0,   31,   32,   33,   17,   18,    2,
    3,    4,    5,    6,    0,    8,    0,   20,    0,   21,
   10,   28,   29,   30,    0,    0,   34,    0,   27,    0,
    0,   25,   12,   13,    0,    0,    0,   23,   15,   16,
};
short gd_dgoto[] = {                                       1,
   10,   11,   12,   13,   14,   15,   16,
};
short gd_sindex[] = {                                      0,
 -241, -256,  -38,  -47,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, -260,    0,  -36,    0, -252,    0,
    0,    0,    0,    0, -249, -248,    0, -247,    0,  -56,
  -32,    0,    0,    0, -245, -235, -257,    0,    0,    0,
};
short gd_rindex[] = {                                      0,
    0,    0,    1,   61,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   10,    0,   19,    0,   37,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   46,
   28,    0,    0,    0,    0,    0,   55,    0,    0,    0,
};
short gd_gindex[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,
};
#define YYTABLESIZE 327
short gd_table[] = {                                      26,
   19,   35,   39,   40,   17,   18,   27,   28,   29,    7,
   25,   30,   31,   32,   36,   37,    2,    3,   24,    4,
    5,    6,    7,    8,    9,   38,    0,   22,    0,    0,
    0,    0,    0,    0,    0,    0,   26,    0,    0,    0,
    0,    0,    0,    0,    0,   11,    0,    0,    0,    0,
    0,    0,    0,    0,   14,    0,    0,    0,    0,    0,
    9,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   33,   34,    0,    0,    0,    0,    0,
   19,   20,   21,    0,   22,   23,   24,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   19,   19,
    0,   19,   19,   19,   19,   19,   19,    7,    7,    0,
    7,    7,    7,    7,    7,    7,   24,   24,    0,   24,
   24,   24,   24,   24,   24,   22,   22,    0,   22,   22,
   22,   22,   22,   22,   26,   26,    0,    0,   26,   26,
   26,   26,   26,   11,   11,    0,    0,   11,   11,   11,
   11,   11,   14,   14,    0,    0,   14,   14,   14,   14,
   14,    9,    0,    0,    0,    9,    9,
};
short gd_check[] = {                                      47,
    0,   58,  260,  261,  261,   44,  267,   44,  261,    0,
   58,  261,  261,  261,   47,  261,  258,  259,    0,  261,
  262,  263,  264,  265,  266,  261,   -1,    0,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  260,  261,   -1,   -1,   -1,   -1,   -1,
  258,  259,  260,   -1,  262,  263,  264,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  258,  259,
   -1,  261,  262,  263,  264,  265,  266,  258,  259,   -1,
  261,  262,  263,  264,  265,  266,  258,  259,   -1,  261,
  262,  263,  264,  265,  266,  258,  259,   -1,  261,  262,
  263,  264,  265,  266,  258,  259,   -1,   -1,  262,  263,
  264,  265,  266,  258,  259,   -1,   -1,  262,  263,  264,
  265,  266,  258,  259,   -1,   -1,  262,  263,  264,  265,
  266,  261,   -1,   -1,   -1,  265,  266,
};
#define YYFINAL 1
#ifndef YYDEBUG
#define YYDEBUG 1
#endif
#define YYMAXTOKEN 267
#if YYDEBUG
char *gd_name[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,"','",0,0,"'/'",0,0,0,0,0,0,0,0,0,0,"':'",0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"ID","MONTH","DAY",
"MERIDIAN","NUMBER","UNIT","MUNIT","SUNIT","ZONE","DAYZONE","AGO",
};
char *gd_rule[] = {
"$accept : timedate",
"timedate :",
"timedate : timedate item",
"item : tspec",
"item : zone",
"item : dtspec",
"item : dyspec",
"item : rspec",
"item : nspec",
"nspec : NUMBER",
"tspec : NUMBER MERIDIAN",
"tspec : NUMBER ':' NUMBER",
"tspec : NUMBER ':' NUMBER MERIDIAN",
"tspec : NUMBER ':' NUMBER NUMBER",
"tspec : NUMBER ':' NUMBER ':' NUMBER",
"tspec : NUMBER ':' NUMBER ':' NUMBER MERIDIAN",
"tspec : NUMBER ':' NUMBER ':' NUMBER NUMBER",
"zone : ZONE",
"zone : DAYZONE",
"dyspec : DAY",
"dyspec : DAY ','",
"dyspec : NUMBER DAY",
"dtspec : NUMBER '/' NUMBER",
"dtspec : NUMBER '/' NUMBER '/' NUMBER",
"dtspec : MONTH NUMBER",
"dtspec : MONTH NUMBER ',' NUMBER",
"dtspec : NUMBER MONTH",
"dtspec : NUMBER MONTH NUMBER",
"rspec : NUMBER UNIT",
"rspec : NUMBER MUNIT",
"rspec : NUMBER SUNIT",
"rspec : UNIT",
"rspec : MUNIT",
"rspec : SUNIT",
"rspec : rspec AGO",
};
#endif
#ifndef YYSTYPE
typedef int YYSTYPE;
#endif
#if YYDEBUG
#include <stdio.h>
#endif

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

int      yydebug;
int      yynerrs;
int      yyerrflag;
int      yychar;
short   *yyssp;
YYSTYPE *yyvsp;
YYSTYPE  yyval;
YYSTYPE  yylval;

/* variables for the parser stack */
static short   *yyss;
static short   *yysslim;
static YYSTYPE *yyvs;
static int      yystacksize;
#line 138 "getdate.y"

static int mdays[12] =
	{31, 0, 31,  30, 31, 30,  31, 31, 30,  31, 30, 31};
#define epoch 1970

extern struct tm *localtime();

static time_t
dateconv(mm, dd, yy, h, m, s, mer, zone, dayflag)
int mm, dd, yy, h, m, s, mer, zone, dayflag;
{
	time_t tod, jdate;
	register int i;

	if (yy < 0) yy = -yy;
	if (yy < 100) yy += 1900;
	mdays[1] =
	    28 + ((yy % 4 == 0 && (yy % 100 != 0 || yy % 400 == 0)) ? 1 : 0);
	if (yy < epoch || yy > 1999 || mm < 1 || mm > 12 ||
		dd < 1 || dd > mdays[--mm]) return (-1);
	jdate = dd-1;
        for (i=0; i<mm; i++) jdate += mdays[i];
	for (i = epoch; i < yy; i++) {
	    jdate += 365 + ((i % 4 == 0 && (i % 100 != 0 || i % 400 == 0)) ? 1 : 0);
	}
	jdate *= daysec;
	jdate += zone * 60L;
	if ((tod = timeconv(h, m, s, mer)) < 0) return (-1);
	jdate += tod;
	if (dayflag == DAYLIGHT
	    || (dayflag == MAYBE && localtime(&jdate)->tm_isdst)) {
	    jdate += -1*60*60;
	}
	return (jdate);
}

static time_t
dayconv(ord, day, now)
int ord, day; time_t now;
{
	register struct tm *loctime;
	time_t tod;

	tod = now;
	loctime = localtime(&tod);
	tod += daysec * ((day - loctime->tm_wday + 7) % 7);
	tod += 7 * daysec * ((ord <= 0) ? ord : (ord - 1));
	return daylcorr(tod, now);
}

static time_t
timeconv(hh, mm, ss, mer)
register int hh, mm, ss, mer;
{
	if (mm < 0 || mm > 59 || ss < 0 || ss > 59) return (-1);
	switch (mer) {
		case AM: if (hh < 1 || hh > 12) return(-1);
			 return (60L * ((hh%12)*60L + mm)+ss);
		case PM: if (hh < 1 || hh > 12) return(-1);
			 return (60L * ((hh%12 +12)*60L + mm)+ss);
		case 24: if (hh < 0 || hh > 23) return (-1);
			 return (60L * (hh*60L + mm)+ss);
		default: return (-1);
	}
}

static time_t
monthadd(sdate, relmonth)
time_t sdate, relmonth;
{
	struct tm *ltime;
	int mm, yy;

	if (relmonth == 0) return 0;
	ltime = localtime(&sdate);
	mm = 12*ltime->tm_year + ltime->tm_mon + relmonth;
	yy = mm/12;
	mm = mm%12 + 1;
	return daylcorr(dateconv(mm, ltime->tm_mday, yy, ltime->tm_hour,
		ltime->tm_min, ltime->tm_sec, 24, ourzone, MAYBE), sdate);
}

static time_t
daylcorr(future, now)
time_t future, now;
{
	int fdayl, nowdayl;

	nowdayl = (localtime(&now)->tm_hour+1) % 24;
	fdayl = (localtime(&future)->tm_hour+1) % 24;
	return (future-now) + 60L * 60L * (nowdayl - fdayl);
}

static char *lptr;

static int gd_lex()
{
	extern int gd_lval;
	int sign;
	register char c;
	register char *p;
	char idbuf[20];
	int pcnt;

	for (;;) {
		while (isspace(*lptr)) lptr++;

		if (isdigit(c = *lptr) || c == '-' || c == '+') {
			if (c== '-' || c == '+') {
				if (c=='-') sign = -1;
				else sign = 1;
				if (!isdigit(*++lptr)) {
					/* gd_lval = sign; return (NUMBER); */
					return gd_lex();/* skip the '-' sign */
				}
			} else sign = 1;
			gd_lval = 0;
			while (isdigit(c = *lptr++)) gd_lval = 10*gd_lval + c - '0';
			gd_lval *= sign;
			lptr--;
			return (NUMBER);

		} else if (isalpha(c)) {
			p = idbuf;
			while (isalpha(c = *lptr++) || c=='.')
				if (p < &idbuf[sizeof(idbuf)-1])
					*p++ = c;
			*p = '\0';
			lptr--;
			return (lookup(idbuf));
		}

		else if (c == '(') {
			pcnt = 0;
			do {
				c = *lptr++;
				if (c == '\0') return(c);
				else if (c == '(') pcnt++;
				else if (c == ')') pcnt--;
			} while (pcnt > 0);
		}

		else return (*lptr++);
	}
}

struct table {
	char *name;
	int type, value;
};

static struct table mdtab[] = {
	{"January", MONTH, 1},
	{"February", MONTH, 2},
	{"March", MONTH, 3},
	{"April", MONTH, 4},
	{"May", MONTH, 5},
	{"June", MONTH, 6},
	{"July", MONTH, 7},
	{"August", MONTH, 8},
	{"September", MONTH, 9},
	{"Sept", MONTH, 9},
	{"October", MONTH, 10},
	{"November", MONTH, 11},
	{"December", MONTH, 12},

	{"Sunday", DAY, 0},
	{"Monday", DAY, 1},
	{"Tuesday", DAY, 2},
	{"Tues", DAY, 2},
	{"Wednesday", DAY, 3},
	{"Wednes", DAY, 3},
	{"Thursday", DAY, 4},
	{"Thur", DAY, 4},
	{"Thurs", DAY, 4},
	{"Friday", DAY, 5},
	{"Saturday", DAY, 6},
	{0, 0, 0}};

#define HRS *60
#define HALFHR 30
static struct table mztab[] = {
	{"a.m.", MERIDIAN, AM},
	{"am", MERIDIAN, AM},
	{"p.m.", MERIDIAN, PM},
	{"pm", MERIDIAN, PM},
	{"nst", ZONE, 3 HRS + HALFHR},		/* Newfoundland */
	{"n.s.t.", ZONE, 3 HRS + HALFHR},
	{"ast", ZONE, 4 HRS},		/* Atlantic */
	{"a.s.t.", ZONE, 4 HRS},
	{"adt", DAYZONE, 4 HRS},
	{"a.d.t.", DAYZONE, 4 HRS},
	{"est", ZONE, 5 HRS},		/* Eastern */
	{"e.s.t.", ZONE, 5 HRS},
	{"edt", DAYZONE, 5 HRS},
	{"e.d.t.", DAYZONE, 5 HRS},
	{"cst", ZONE, 6 HRS},		/* Central */
	{"c.s.t.", ZONE, 6 HRS},
	{"cdt", DAYZONE, 6 HRS},
	{"c.d.t.", DAYZONE, 6 HRS},
	{"mst", ZONE, 7 HRS},		/* Mountain */
	{"m.s.t.", ZONE, 7 HRS},
	{"mdt", DAYZONE, 7 HRS},
	{"m.d.t.", DAYZONE, 7 HRS},
	{"pst", ZONE, 8 HRS},		/* Pacific */
	{"p.s.t.", ZONE, 8 HRS},
	{"pdt", DAYZONE, 8 HRS},
	{"p.d.t.", DAYZONE, 8 HRS},
	{"yst", ZONE, 9 HRS},		/* Yukon */
	{"y.s.t.", ZONE, 9 HRS},
	{"ydt", DAYZONE, 9 HRS},
	{"y.d.t.", DAYZONE, 9 HRS},
	{"hst", ZONE, 10 HRS},		/* Hawaii */
	{"h.s.t.", ZONE, 10 HRS},
	{"hdt", DAYZONE, 10 HRS},
	{"h.d.t.", DAYZONE, 10 HRS},

	{"gmt", ZONE, 0 HRS},
	{"g.m.t.", ZONE, 0 HRS},
	{"bst", DAYZONE, 0 HRS},		/* British Summer Time */
	{"b.s.t.", DAYZONE, 0 HRS},
	{"eet", ZONE, 0 HRS},		/* European Eastern Time */
	{"e.e.t.", ZONE, 0 HRS},
	{"eest", DAYZONE, 0 HRS},	/* European Eastern Summer Time */
	{"e.e.s.t.", DAYZONE, 0 HRS},
	{"met", ZONE, -1 HRS},		/* Middle European Time */
	{"m.e.t.", ZONE, -1 HRS},
	{"mest", DAYZONE, -1 HRS},	/* Middle European Summer Time */
	{"m.e.s.t.", DAYZONE, -1 HRS},
	{"wet", ZONE, -2 HRS },		/* Western European Time */
	{"w.e.t.", ZONE, -2 HRS },
	{"west", DAYZONE, -2 HRS},	/* Western European Summer Time */
	{"w.e.s.t.", DAYZONE, -2 HRS},

	{"jst", ZONE, -9 HRS},		/* Japan Standard Time */
	{"j.s.t.", ZONE, -9 HRS},	/* Japan Standard Time */
					/* No daylight savings time */

	{"aest", ZONE, -10 HRS},	/* Australian Eastern Time */
	{"a.e.s.t.", ZONE, -10 HRS},
	{"aesst", DAYZONE, -10 HRS},	/* Australian Eastern Summer Time */
	{"a.e.s.s.t.", DAYZONE, -10 HRS},
	{"acst", ZONE, -(9 HRS + HALFHR)},	/* Australian Central Time */
	{"a.c.s.t.", ZONE, -(9 HRS + HALFHR)},
	{"acsst", DAYZONE, -(9 HRS + HALFHR)},	/* Australian Central Summer */
	{"a.c.s.s.t.", DAYZONE, -(9 HRS + HALFHR)},
	{"awst", ZONE, -8 HRS},		/* Australian Western Time */
	{"a.w.s.t.", ZONE, -8 HRS},	/* (no daylight time there, I'm told */
	{0, 0, 0}};

static struct table unittb[] = {
	{"year", MUNIT, 12},
	{"month", MUNIT, 1},
	{"fortnight", UNIT, 14*24*60},
	{"week", UNIT, 7*24*60},
	{"day", UNIT, 1*24*60},
	{"hour", UNIT, 60},
	{"minute", UNIT, 1},
	{"min", UNIT, 1},
	{"second", SUNIT, 1},
	{"sec", SUNIT, 1},
	{0, 0, 0}};

static struct table othertb[] = {
	{"tomorrow", UNIT, 1*24*60},
	{"yesterday", UNIT, -1*24*60},
	{"today", UNIT, 0},
	{"now", UNIT, 0},
	{"last", NUMBER, -1},
	{"this", UNIT, 0},
	{"next", NUMBER, 2},
	{"first", NUMBER, 1},
	/* {"second", NUMBER, 2}, */
	{"third", NUMBER, 3},
	{"fourth", NUMBER, 4},
	{"fifth", NUMBER, 5},
	{"sixth", NUMBER, 6},
	{"seventh", NUMBER, 7},
	{"eigth", NUMBER, 8},
	{"ninth", NUMBER, 9},
	{"tenth", NUMBER, 10},
	{"eleventh", NUMBER, 11},
	{"twelfth", NUMBER, 12},
	{"ago", AGO, 1},
	{0, 0, 0}};

static struct table milzone[] = {
	{"a", ZONE, 1 HRS},
	{"b", ZONE, 2 HRS},
	{"c", ZONE, 3 HRS},
	{"d", ZONE, 4 HRS},
	{"e", ZONE, 5 HRS},
	{"f", ZONE, 6 HRS},
	{"g", ZONE, 7 HRS},
	{"h", ZONE, 8 HRS},
	{"i", ZONE, 9 HRS},
	{"k", ZONE, 10 HRS},
	{"l", ZONE, 11 HRS},
	{"m", ZONE, 12 HRS},
	{"n", ZONE, -1 HRS},
	{"o", ZONE, -2 HRS},
	{"p", ZONE, -3 HRS},
	{"q", ZONE, -4 HRS},
	{"r", ZONE, -5 HRS},
	{"s", ZONE, -6 HRS},
	{"t", ZONE, -7 HRS},
	{"u", ZONE, -8 HRS},
	{"v", ZONE, -9 HRS},
	{"w", ZONE, -10 HRS},
	{"x", ZONE, -11 HRS},
	{"y", ZONE, -12 HRS},
	{"z", ZONE, 0 HRS},
	{0, 0, 0}};

static int lookup(id)
char *id;
{
#define gotit (gd_lval = i->value , i->type)
#define getid for(j = idvar, k = id; (*j++ = *k++) != '\0'; /* void */)

	char idvar[20];
	register char *j, *k;
	register struct table *i;
	int abbrev;

	getid;
	if (strlen(idvar) == 3) {
	    abbrev = 1;
	}
	else if (strlen(idvar) == 4 && idvar[3] == '.') {
		abbrev = 1;
		idvar[3] = '\0';
	}
	else abbrev = 0;

	if (islower(*idvar)) *idvar = toupper(*idvar);

	for (i = mdtab; i->name; i++) {
		k = idvar;
		for (j = i->name; *j++ == *k++;) {
			if (abbrev && j==i->name+3) return gotit;
			if (j[-1] == 0) return gotit;
		}
	}

	getid;
	for (i = mztab; i->name; i++)
		if (strcmp(i->name, idvar) == 0) return gotit;

	for (j = idvar; *j; j++)
		if (isupper(*j)) *j = tolower(*j);
	for (i=mztab; i->name; i++)
		if (strcmp(i->name, idvar) == 0) return gotit;

	getid;
	for (i=unittb; i->name; i++)
		if (strcmp(i->name, idvar) == 0) return gotit;

	if (idvar[strlen(idvar)-1] == 's')
		idvar[strlen(idvar)-1] = '\0';
	for (i=unittb; i->name; i++)
		if (strcmp(i->name, idvar) == 0) return gotit;

	getid;
	for (i = othertb; i->name; i++)
		if (strcmp(i->name, idvar) == 0) return gotit;

	getid;
	if (strlen(idvar) == 1 && isalpha(*idvar)) {
		if (isupper(*idvar)) *idvar = tolower(*idvar);
		for (i = milzone; i->name; i++)
			if (strcmp(i->name, idvar) == 0) return gotit;
	}

	return(ID);
}

date_info getdate(p, now)
char *p;
struct timeb *now;
{
#define mcheck(f)	if (f>1) err++
    int gd_parse();
    time_t monthadd();
    int err;
    struct tm *lt;
    struct timeb ftz;
    date_info value;
    
    time_t sdate, tod;
    
    lptr = p;
    if (now == ((struct timeb *) NULL)) {
	now = &ftz;
	ftime(&ftz);
    }
    lt = localtime(&now->time);
    year = lt->tm_year;
    month = lt->tm_mon+1;
    day = lt->tm_mday;
    relsec = 0; relmonth = 0;
    timeflag = zoneflag = dateflag = dayflag = relflag = 0;
    ourzone = now->timezone;
    dylight = MAYBE;
    hh = mm = ss = 0;
    merid = 24;
    
    if (err = gd_parse()) {
	value.seconds = -1;
	return value;
    }
    
    mcheck(timeflag);
    mcheck(zoneflag);
    mcheck(dateflag);
    mcheck(dayflag);
    
    if (err) {
	value.seconds = -1;
	return value;
    }
    if (dateflag || timeflag || dayflag) {
	sdate = dateconv(month,day,year,hh,mm,ss,merid,ourzone,dylight);
	if (sdate < 0) {
	    value.seconds = -1;
	    return value;
	}
    }
    else {
	sdate = now->time;
	if (relflag == 0)
	    sdate -= (lt->tm_sec + lt->tm_min * 60
		      + lt->tm_hour * (60L * 60L));
    }
    
    sdate += relsec;
    sdate += monthadd(sdate, relmonth);
    
    if (dayflag && !dateflag) {
	tod = dayconv(dayord, dayreq, sdate);
	sdate += tod;
    }
    
#if defined(DEBUG2)
    (void) fprintf(stderr, "timeflag = %d, zoneflag = %d, dateflag = %d\n",
		   timeflag, zoneflag, dateflag);
    (void) fprintf(stderr, "dayflag = %d, relflag = %d\n",
		   dayflag, relflag);
    (void) fprintf(stderr, "seconds = %lu\n", sdate);
#endif
    
    value.seconds = sdate;
    return value;
}

/*ARGSUSED*/
void gd_error(char *s)
{
    return;
}
#line 760 "y.tab.c"
/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(void)
{
    int newsize, i;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = yystacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = yyssp - yyss;
    newss = (yyss != 0)
          ? (short *)realloc(yyss, newsize * sizeof(*newss))
          : (short *)malloc(newsize * sizeof(*newss));
    if (newss == 0)
        return -1;

    yyss  = newss;
    yyssp = newss + i;
    newvs = (yyvs != 0)
          ? (YYSTYPE *)realloc(yyvs, newsize * sizeof(*newvs))
          : (YYSTYPE *)malloc(newsize * sizeof(*newvs));
    if (newvs == 0)
        return -1;

    yyvs = newvs;
    yyvsp = newvs + i;
    yystacksize = newsize;
    yysslim = yyss + newsize - 1;
    return 0;
}

#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab
int
yyparse(void)
{
    register int yym, yyn, yystate;
#if YYDEBUG
    register const char *yys;

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

    if (yyss == NULL && yygrowstack()) goto yyoverflow;
    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
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
        if (yyssp >= yysslim && yygrowstack())
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
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

#ifdef lint
    goto yyerrlab;
#endif

yyerrlab:
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yysslim && yygrowstack())
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
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
        yyval = yyvsp[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
    switch (yyn)
    {
case 3:
#line 50 "getdate.y"

		{timeflag++;}
break;
case 4:
#line 52 "getdate.y"

		{zoneflag++;}
break;
case 5:
#line 54 "getdate.y"

		{dateflag++;}
break;
case 6:
#line 56 "getdate.y"

		{dayflag++;}
break;
case 7:
#line 58 "getdate.y"

		{relflag++;}
break;
case 9:
#line 62 "getdate.y"

		{if (timeflag && dateflag && !relflag) year = yyvsp[0];
		else {timeflag++;hh = yyvsp[0]/100;mm = yyvsp[0]%100;ss = 0;merid = 24;}}
break;
case 10:
#line 66 "getdate.y"

		{hh = yyvsp[-1]; mm = 0; ss = 0; merid = yyvsp[0];}
break;
case 11:
#line 68 "getdate.y"

		{hh = yyvsp[-2]; mm = yyvsp[0]; merid = 24;}
break;
case 12:
#line 70 "getdate.y"

		{hh = yyvsp[-3]; mm = yyvsp[-1]; merid = yyvsp[0];}
break;
case 13:
#line 72 "getdate.y"

		{hh = yyvsp[-3]; mm = yyvsp[-1]; merid = 24;
		dylight = STANDARD; ourzone = yyvsp[0]%100 + 60*yyvsp[0]/100;}
break;
case 14:
#line 75 "getdate.y"

		{hh = yyvsp[-4]; mm = yyvsp[-2]; ss = yyvsp[0]; merid = 24;}
break;
case 15:
#line 77 "getdate.y"

		{hh = yyvsp[-5]; mm = yyvsp[-3]; ss = yyvsp[-1]; merid = yyvsp[0];}
break;
case 16:
#line 79 "getdate.y"

		{hh = yyvsp[-5]; mm = yyvsp[-3]; ss = yyvsp[-1]; merid = 24;
		dylight = STANDARD; ourzone = yyvsp[0]%100 + 60*yyvsp[0]/100;}
break;
case 17:
#line 83 "getdate.y"

		{ourzone = yyvsp[0]; dylight = STANDARD;}
break;
case 18:
#line 85 "getdate.y"

		{ourzone = yyvsp[0]; dylight = DAYLIGHT;}
break;
case 19:
#line 88 "getdate.y"

		{dayord = 1; dayreq = yyvsp[0];}
break;
case 20:
#line 90 "getdate.y"

		{dayord = 1; dayreq = yyvsp[-1];}
break;
case 21:
#line 92 "getdate.y"

		{dayord = yyvsp[-1]; dayreq = yyvsp[0];}
break;
case 22:
#line 95 "getdate.y"

		{month = yyvsp[-2]; day = yyvsp[0];}
break;
case 23:
#line 97 "getdate.y"

		{
		    /*
		     * HACK ALERT!!!!
		     * The 1000 is a magic number to attempt to force
		     * use of 4 digit years if year/month/day can be
		     * parsed. This was only done for backwards
		     * compatibility in rh.
		     */
		    if (yyvsp[-4] > 1000) {
			year = yyvsp[-4]; month = yyvsp[-2]; day = yyvsp[0];
		    }
		    else {
			month = yyvsp[-4]; day = yyvsp[-2]; year = yyvsp[0];
		    }
		}
break;
case 24:
#line 113 "getdate.y"

		{month = yyvsp[-1]; day = yyvsp[0];}
break;
case 25:
#line 115 "getdate.y"

		{month = yyvsp[-3]; day = yyvsp[-2]; year = yyvsp[0];}
break;
case 26:
#line 117 "getdate.y"

		{month = yyvsp[0]; day = yyvsp[-1];}
break;
case 27:
#line 119 "getdate.y"

		{month = yyvsp[-1]; day = yyvsp[-2]; year = yyvsp[0];}
break;
case 28:
#line 123 "getdate.y"

		{relsec +=  60L * yyvsp[-1] * yyvsp[0];}
break;
case 29:
#line 125 "getdate.y"

		{relmonth += yyvsp[-1] * yyvsp[0];}
break;
case 30:
#line 127 "getdate.y"

		{relsec += yyvsp[-1];}
break;
case 31:
#line 129 "getdate.y"

		{relsec +=  60L * yyvsp[0];}
break;
case 32:
#line 131 "getdate.y"

		{relmonth += yyvsp[0];}
break;
case 33:
#line 133 "getdate.y"

		{relsec++;}
break;
case 34:
#line 135 "getdate.y"

		{relsec = -relsec; relmonth = -relmonth;}
break;
#line 1113 "y.tab.c"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
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
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yysslim && yygrowstack())
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;

yyoverflow:
    yyerror("yacc stack overflow");

yyabort:
    return (1);

yyaccept:
    return (0);
}

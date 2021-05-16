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
extern YYSTYPE rh_lval;

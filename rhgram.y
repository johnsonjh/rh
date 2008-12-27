%{

/*
 * rhgram.y
 *
 * Written by:	Rick Ohnemus	(rick@sterling.com)
 */

#if !defined(lint)
static char rcsid[] = "$Id: rhgram.y,v 1.3 1994/01/26 16:17:54 rick Exp $";
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

%}

%union {
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
}

%token <sym>    BLTIN
%token <di>     DATESPEC
%token          EQ
%token <sym>    FIELD
%token <str>    FILENAME_PATTERN
%token <di>	FILETIME_FIELD
%token <str>    FSTYPE
%token <sym>    FUNCTION
%token          GE
%token <sym>    IDENTIFIER
%token          LE
%token          LOGAND
%token          LOGOR
%token          LSHIFT
%token          NE
%token <value>  NUMBER
%token <sym>    PARAM
%token          RETURN
%token          RSHIFT
%token <sym>	SYSTEM_CALL
%token <sym>    TIME_FIELD

%right	'?' ':'
%left	LOGOR
%left	LOGAND
%left	'|'
%left	'^'
%left	'&'
%left	EQ NE
%left	'<' '>' LE GE
%left	RSHIFT LSHIFT
%left	'+' '-'
%left	'*' '/' '%'
%nonassoc UNIMINUS
%nonassoc '!'
%nonassoc '~'

%type <tree>	expression
%type <tree>	exprlist
%type <tree>	factor
%type <tree>	function_body
%type <value>	idlist
%type <tree>	paramlist

%%

program:	  functions
			{
			    rh_lex_finish();
			}
		| functions expression
			{
			    rh_lex_finish();
			    if (Start_expr == (node *) NULL) {
				Start_expr = $2;
			    }
			    else {
				error("too many start expressions specified");
			    }
			}
		| functions expression ';'
			{
			    rh_lex_finish();
			    if (Start_expr == (node *) NULL) {
				Start_expr = $2;
			    }
			    else {
				error("too many start expressions specified");
			    }
			}
		;

functions: 	  /* empty */
		| functions function
		;

function:	  IDENTIFIER
			{
			    $1->type = FUNCTION;
			    $1->func = c_func;
			    $1->value = -1;
			    $1->tree = (node *) NULL;
			}
		  function_body
			{
			    $1->tree = $3;
			    $1->num_params = 0;
			}
		| IDENTIFIER
			{
			    $1->type = FUNCTION;
			    $1->func = c_func;
			    $1->value = -1;
			    $1->tree = (node *) NULL;
			}
		  '(' idlist ')'
			{
			    $1->num_params = $4;
			}
		  function_body
			{
			    symbol *sym;

			    $1->tree = $7;

			    while (Symbol_table->type == PARAM) {
				if (Symbol_table->ref_count == 0
				    && Issue_warnings) {
				    warning("warning: function '%s' parameter '%s' is not referenced",
					    $1->name, Symbol_table->name);
				}
				sym = Symbol_table;
				Symbol_table = Symbol_table->next;
				free(sym->name);
				free((char *) sym);
			    }
			}
		| error
			{
			    rh_error("expected function definition");
			}
		;

idlist:		  /* empty */
			{
			    $$ = 0;
			}
		| IDENTIFIER
			{
			    $1->type = PARAM;
			    $1->func = c_param;
			    $1->value = 0;
			    $1->ref_count = 0;
			    $$ = 1;
			}
		| idlist ',' IDENTIFIER
			{
			    $3->type = PARAM;
			    $3->func = c_param;
			    $3->value = $1;
			    $3->ref_count = 0;
			    $$ = $1 + 1;
			}
		| error
			{
			    rh_error("expected identifier");
			}
		;

function_body:	  '{' return expression ';' '}'
			{ $$ = $3; }
		| '{' return expression '}'
			{ $$ = $3; }
		;

return:		  /* empty */
		| RETURN
		;

expression:	  factor
			{ $$ = $1; }
		| expression '?' expression ':' expression
			{
			    if ($1->code.func == c_number
				&& $3->code.func == c_number
				&& $5->code.func == c_number) {
				$$ = $1;
				$$->code.value.l =
				    $1->code.value.l ? $3->code.value.l : $5->code.value.l;
				free((char *) $3);
				free((char *) $5);
			    }
			    else {
				value.l = 0L;
				$$ = make_node(TERTIARY, &value);
				$$->left = $1;
				$$->middle = $3;
				$$->right = $5;
			    }
			}
		| expression LOGOR expression
			{
			    if ($1->code.func == c_number
				&& $3->code.func == c_number) {
				$$ = $1;
				$$->code.value.l =
				    $1->code.value.l || $3->code.value.l;
				free((char *) $3);
			    }
			    else {
				value.l = 0L;
				$$ = make_node(c_or, &value);
				$$->left = $1;
				$$->right = $3;
			    }
			}
		| expression LOGAND expression
			{
			    if ($1->code.func == c_number
				&& $3->code.func == c_number) {
				$$ = $1;
				$$->code.value.l =
				    $1->code.value.l && $3->code.value.l;
				free((char *) $3);
			    }
			    else {
				value.l = 0L;
				$$ = make_node(c_and, &value);
				$$->left = $1;
				$$->right = $3;
			    }
			}
		| expression '|' expression
			{
			    if ($1->code.func == c_number
				&& $3->code.func == c_number) {
				$$ = $1;
				$$->code.value.l =
				    $1->code.value.l | $3->code.value.l;
				free((char *) $3);
			    }
			    else {
				value.l = 0L;
				$$ = make_node(c_bor, &value);
				$$->left = $1;
				$$->right = $3;
			    }
			}
		| expression '^' expression
			{
			    if ($1->code.func == c_number
				&& $3->code.func == c_number) {
				$$ = $1;
				$$->code.value.l =
				    $1->code.value.l ^ $3->code.value.l;
				free((char *) $3);
			    }
			    else {
				value.l = 0L;
				$$ = make_node(c_bxor, &value);
				$$->left = $1;
				$$->right = $3;
			    }
			}
		| expression '&' expression
			{
			    if ($1->code.func == c_number
				&& $3->code.func == c_number) {
				$$ = $1;
				$$->code.value.l =
				    $1->code.value.l & $3->code.value.l;
				free((char *) $3);
			    }
			    else {
				value.l = 0L;
				$$ = make_node(c_band, &value);
				$$->left = $1;
				$$->right = $3;
			    }
			}
		| expression EQ expression
			{
			    if ($1->code.func == c_number
				&& $3->code.func == c_number) {
				$$ = $1;
				$$->code.value.l =
				    $1->code.value.l == $3->code.value.l;
				free((char *) $3);
			    }
			    else {
				value.l = 0L;
				$$ = make_node(c_eq, &value);
				$$->left = $1;
				$$->right = $3;
			    }
			}
		| expression NE expression
			{
			    if ($1->code.func == c_number
				&& $3->code.func == c_number) {
				$$ = $1;
				$$->code.value.l =
				    $1->code.value.l != $3->code.value.l;
				free((char *) $3);
			    }
			    else {
				value.l = 0L;
				$$ = make_node(c_ne, &value);
				$$->left = $1;
				$$->right = $3;
			    }
			}
		| expression '<' expression
			{
			    if ($1->code.func == c_number
				&& $3->code.func == c_number) {
				$$ = $1;
				$$->code.value.l =
				    $1->code.value.l < $3->code.value.l;
				free((char *) $3);
			    }
			    else {
				value.l = 0L;
				$$ = make_node(c_lt, &value);
				$$->left = $1;
				$$->right = $3;
			    }
			}
		| expression '>' expression
			{
			    if ($1->code.func == c_number
				&& $3->code.func == c_number) {
				$$ = $1;
				$$->code.value.l =
				    $1->code.value.l > $3->code.value.l;
				free((char *) $3);
			    }
			    else {
				value.l = 0L;
				$$ = make_node(c_gt, &value);
				$$->left = $1;
				$$->right = $3;
			    }
			}
		| expression LE expression
			{
			    if ($1->code.func == c_number
				&& $3->code.func == c_number) {
				$$ = $1;
				$$->code.value.l =
				    $1->code.value.l <= $3->code.value.l;
				free((char *) $3);
			    }
			    else {
				value.l = 0L;
				$$ = make_node(c_le, &value);
				$$->left = $1;
				$$->right = $3;
			    }
			}
		| expression GE expression
			{
			    if ($1->code.func == c_number
				&& $3->code.func == c_number) {
				$$ = $1;
				$$->code.value.l =
				    $1->code.value.l >= $3->code.value.l;
				free((char *) $3);
			    }
			    else {
				value.l = 0L;
				$$ = make_node(c_ge, &value);
				$$->left = $1;
				$$->right = $3;
			    }
			}
		| expression RSHIFT expression
			{
			    if ($1->code.func == c_number
				&& $3->code.func == c_number) {
				$$ = $1;
				$$->code.value.l =
				    $1->code.value.l >> $3->code.value.l;
				free((char *) $3);
			    }
			    else {
				value.l = 0L;
				$$ = make_node(c_rshift, &value);
				$$->left = $1;
				$$->right = $3;
			    }
			}
		| expression LSHIFT expression
			{
			    if ($1->code.func == c_number
				&& $3->code.func == c_number) {
				$$ = $1;
				$$->code.value.l =
				    $1->code.value.l << $3->code.value.l;
				free((char *) $3);
			    }
			    else {
				value.l = 0L; 
				$$ = make_node(c_lshift, &value);
				$$->left = $1;
				$$->right = $3;
			    }
			}
		| expression '+' expression
			{
			    if ($1->code.func == c_number
				&& $3->code.func == c_number) {
				$$ = $1;
				$$->code.value.l =
				    $1->code.value.l + $3->code.value.l;
				free((char *) $3);
			    }
			    else {
				value.l = 0L;
				$$ = make_node(c_plus, &value);
				$$->left = $1;
				$$->right = $3;
			    }
			}
		| expression '-' expression
			{
			    if ($1->code.func == c_number
				&& $3->code.func == c_number) {
				$$ = $1;
				$$->code.value.l =
				    $1->code.value.l - $3->code.value.l;
				free((char *) $3);
			    }
			    else {
				value.l = 0L;
				$$ = make_node(c_minus, &value);
				$$->left = $1;
				$$->right = $3;
			    }
			}
		| expression '*' expression
			{
			    if ($1->code.func == c_number
				&& $3->code.func == c_number) {
				$$ = $1;
				$$->code.value.l =
				    $1->code.value.l * $3->code.value.l;
				free((char *) $3);
			    }
			    else {
				value.l = 0L;
				$$ = make_node(c_mul, &value);
				$$->left = $1;
				$$->right = $3;
			    }
			}
		| expression '/' expression
			{
			    if ($1->code.func == c_number
				&& $3->code.func == c_number) {
				$$ = $1;
				$$->code.value.l =
				    $1->code.value.l / $3->code.value.l;
				free((char *) $3);
			    }
			    else {
				value.l = 0L;
				$$ = make_node(c_div, &value);
				$$->left = $1;
				$$->right = $3;
			    }
			}
		| expression '%' expression
			{
			    if ($1->code.func == c_number
				&& $3->code.func == c_number) {
				$$ = $1;
				$$->code.value.l =
				    $1->code.value.l % $3->code.value.l;
				free((char *) $3);
			    }
			    else {
				value.l = 0L;
				$$ = make_node(c_mod, &value);
				$$->left = $1;
				$$->right = $3;
			    }
			}
		| '~' expression
			{
			    if ($2->code.func == c_number) {
				$$ = $2;
				$$->code.value.l = ~ $$->code.value.l;
			    }
			    else {
				value.l = 0L;
				$$ = make_node(c_bnot, &value);
				$$->right = $2;
			    }
			}
		| '!' expression
			{
			    if ($2->code.func == c_number) {
				$$ = $2;
				$$->code.value.l = ! $$->code.value.l;
			    }
			    else {
				value.l = 0L;
				$$ = make_node(c_not, &value);
				$$->right = $2;
			    }
			}
		| '-' expression	%prec UNIMINUS
			{
			    if ($2->code.func == c_number) {
				$$ = $2;
				$$->code.value.l = - $$->code.value.l;
			    }
			    else {
				value.l = 0L;
				$$ = make_node(c_uniminus, &value);
				$$->right = $2;
			    }
			}
		;

factor:		  FUNCTION
			{
			    $<line_info>$.line = SAVESTR(Line);
			    $<line_info>$.line_num = Line_num;
			    $<line_info>$.line_pos = Line_pos - strlen($1->name);
			}
		  paramlist
			{
			    if ($1->num_params != $3->count) {
				free(Line);
				Line = $<line_info>2.line;
				Line_num = -($<line_info>2.line_num);
				Line_pos = $<line_info>2.line_pos;
				rh_error("function called with wrong number of arguments");
			    }
			    if ($1->tree != (node *) NULL
				&& $1->tree->left == (node *) NULL
				&& $1->tree->right == (node *) NULL) {
				int i;
				
				if ($1->tree->code.func == c_param) {
				    for (i = 0; i < $3->count; i++) {
					if (i == $1->tree->code.value.param) {
					    $$ = $3->trees[i];
					}
					else {
					    free_tree($3->trees[i]);
					}
				    }
				}
				else {
				    $$ = copy_tree($1->tree);
				    for (i = 0; i < $3->count; i++) {
					free_tree($3->trees[i]);
				    }
				}
				if ($3->trees != (node **) NULL) {
				    free((char *) $3->trees);
				}
				free((char *) $3);
			    }
			    else {
				$$ = $3;
				$$->code.func = UNRES_FUNC;
				$$->code.value.func_name = $1->name;
			    }
			    free($<line_info>2.line);
			}
		| SYSTEM_CALL
			{
			    $<line_info>$.line = SAVESTR(Line);
			    $<line_info>$.line_num = Line_num;
			    $<line_info>$.line_pos = Line_pos - strlen($1->name);
			}
		  paramlist
			{
			    if ($1->num_params != $3->count) {
				free(Line);
				Line = $<line_info>2.line;
				Line_num = -($<line_info>2.line_num);
				Line_pos = $<line_info>2.line_pos;
				rh_error("function called with wrong number of arguments");
			    }
			    $$ = $3;
			    $$->code.func = c_syscall;
			    $$->code.value.sym = $1;
			    free($<line_info>2.line);
			}
		| '(' expression ')'
			{ $$ = $2; }
		| NUMBER
			{
			    value.l = $1;
			    $$ = make_node(c_number, &value);
			}
		| FILENAME_PATTERN
			{
			    if (attr.dot_special && strcmp($1, "*") == 0) {
				value.l = 1L;
				$$ = make_node(c_number, &value);
			    }
			    else {
				value.pattern = $1;
				if (pattern($1)) {
				    $$ = make_node(c_pattern, &value);
				}
				else if (attr.ignore_case) {
				    $$ = make_node(c_fnigncase, &value);
				}
				else {
				    $$ = make_node(c_fncase, &value);
				}
			    }
			}
		| FSTYPE
			{
			    value.fstype = $1;
			    $$ = make_node(c_fstype, &value);
			}
		| BLTIN
			{
			    value.l = $1->value;
			    $$ = make_node($1->func, &value);
			}
		| FIELD
			{
			    value.l = $1->value;
			    $$ = make_node($1->func, &value);
			}
		| PARAM
			{
			    value.param = $1->value;
			    $$ = make_node(c_param, &value);
			    $1->ref_count++;
			}
		| IDENTIFIER
			{ rh_error("undefined identifier"); }
		| DATESPEC
			{
			    value.di = $1;
			    $$ = make_node(c_date, &value);
			}
		| FILETIME_FIELD
			{
			    value.di = $1;
			    $$ = make_node(c_date, &value);
			}
		| TIME_FIELD
			{
			    value.l = $1->value;
			    $$ = make_node($1->func, &value);
			}
		;

paramlist:	  /* empty */
			{
			    value.l = 0L;
			    $$ = make_node(EXPR_LIST, &value);
			    $$->count = 0;
			    $$->trees = (node **) NULL;
			}
		| '(' exprlist ')'
			{ $$ = $2; }

exprlist:	  /* empty */
			{
			    value.l = 0L;
			    $$ = make_node(EXPR_LIST, &value);
			    $$->count = 0;
			    $$->trees = (node **) NULL;
			}
		| expression
			{
			    value.l = 0L;
			    $$ = make_node(EXPR_LIST, &value);
			    $$->count = 1;
			    $$->trees = ALLOCATE(sizeof(node *));
			    $$->trees[0] = $1;
			}
		| exprlist ',' expression
			{
			    $$ = $1;
			    $$->count++;
			    $$->trees =
				REALLOCATE($$->trees,
					   (sizeof(node *)) * $$->count);
			    $$->trees[$$->count - 1] = $3;
			}
		;

%%

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

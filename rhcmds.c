/*
 * rhcmds.c
 *
 * Written by:	Ken Stauffer
 *		Rick Ohnemus	(rick@sterling.com)
 * 
 * This file contains the functions that do the evaluation of
 * the stack program.
 * These functions are simple, and behave like RPN operators, that is
 * they use the last two values on the stack, apply an operator
 * and push the result. Similarly for unary ops.
 *
 */

#if !defined(lint)
static char rcsid[] = "$Id: rhcmds.c,v 1.3 2011/12/31 02:41:07 vandys Exp $";
#endif

#include "rh.h"
#include "rhdata.h"
#include "sysinfo.h"

void c_or(instr_value *v)
{
    Stack[SP - 2] = Stack[SP - 2] || Stack[SP - 1];
    SP--;
    return;
}

void c_and(instr_value *v)
{
    Stack[SP - 2] = Stack[SP - 2] && Stack[SP - 1];
    SP--;
    return;
}

void c_le(instr_value *v)
{
    Stack[SP - 2] = Stack[SP - 2] <= Stack[SP - 1];
    SP--;
    return;
}

void c_lt(instr_value *v)
{
    Stack[SP - 2] = Stack[SP - 2] < Stack[SP - 1];
    SP--;
    return;
}

void c_ge(instr_value *v)
{
    Stack[SP - 2] = Stack[SP - 2] >= Stack[SP - 1];
    SP--;
    return;
}

void c_gt(instr_value *v)
{
    Stack[SP - 2] = Stack[SP - 2] > Stack[SP - 1];
    SP--;
    return;
}

void c_ne(instr_value *v)
{
    Stack[SP - 2] = Stack[SP - 2] != Stack[SP - 1];
    SP--;
    return;
}

void c_eq(instr_value *v)
{
    Stack[SP - 2] = Stack[SP - 2] == Stack[SP - 1];
    SP--;
    return;
}

void c_bor(instr_value *v)
{
    Stack[SP - 2] = Stack[SP - 2] | Stack[SP - 1];
    SP--;
    return;
}

void c_band(instr_value *v)
{
    Stack[SP - 2] = Stack[SP - 2] & Stack[SP - 1];
    SP--;
    return;
}

void c_bxor(instr_value *v)
{
    Stack[SP - 2] = Stack[SP - 2] ^ Stack[SP - 1];
    SP--;
    return;
}

void c_lshift(instr_value *v)
{
    Stack[SP - 2] = Stack[SP - 2] << Stack[SP - 1];
    SP--;
    return;
}

void c_rshift(instr_value *v)
{
    Stack[SP - 2] = Stack[SP - 2] >> Stack[SP - 1];
    SP--;
    return;
}

void c_plus(instr_value *v)
{
    Stack[SP - 2] = Stack[SP - 2] + Stack[SP - 1];
    SP--;
    return;
}

void c_mul(instr_value *v)
{
    Stack[SP - 2] = Stack[SP - 2] * Stack[SP - 1];
    SP--;
    return;
}

void c_minus(instr_value *v)
{
    Stack[SP - 2] = Stack[SP - 2]  -  Stack[SP-1];
    SP--;
    return;
}

void c_div(instr_value *v)
{
    Stack[SP - 2] = Stack[SP - 2] / Stack[SP - 1];
    SP--;
    return;
}

void c_mod(instr_value *v)
{
    Stack[SP - 2] = Stack[SP - 2] % Stack[SP - 1];
    SP--;
    return;
}


/* unary instructions */

void c_not(instr_value *v)
{
    Stack[SP - 1] = !Stack[SP - 1];
    return;
}

void c_bnot(instr_value *v)
{
    Stack[SP - 1] = ~Stack[SP - 1];
    return;
}

void c_uniminus(instr_value *v)
{
    Stack[SP - 1] = -Stack[SP - 1];
    return;
}


/* trinary operator ?: */

void c_qm(instr_value *v)
{
    PC = (Stack[SP - 1]) ? PC : v->pc;
    SP--;
    return;
}

void c_colon(instr_value *v)
{
    PC = v->pc;
    return;
}


/* accessing a parameter */

void c_param(instr_value *v)
{
    Stack[SP++] = Stack[FP + v->param];
    return;
}


/* calling a function */

void c_func(instr_value *v)
{
    Stack[SP++] = PC;
    Stack[SP++] = FP;
    PC = v->pc;
    FP = SP - (StackProgram[PC].value.pc + 2);
    return;
}


/* returning from a function */

void c_return(instr_value *v)
{
    PC = Stack[SP - 3];
    FP = Stack[SP - 2];
    Stack[SP - (3 + v->param)] = Stack[SP - 1];
    SP -= (2 + v->param);
    return;
}


/* operand functions */

void c_nop(instr_value *v)
{
    return;
}

void c_date(instr_value *v)
{
    Stack[SP++] = v->di->seconds;
    return;
}

void c_number(instr_value *v)
{
    Stack[SP++] = v->l;
    return;
}

void c_atime(instr_value *v)
{
    Stack[SP++] = attr.buf->st_atime;
    return;
}

void c_ctime(instr_value *v)
{
    Stack[SP++] = attr.buf->st_ctime;
    return;
}

void c_dev(instr_value *v)
{
    Stack[SP++] = attr.buf->st_dev;
    return;
}

void c_gid(instr_value *v)
{
    Stack[SP++] = attr.buf->st_gid;
    return;
}

void c_ino(instr_value *v)
{
    Stack[SP++] = attr.buf->st_ino;
    return;
}

void c_isblk(instr_value *v)
{
    Stack[SP++] = S_ISBLK(attr.buf->st_mode);
    return;
}

void c_ischr(instr_value *v)
{
    Stack[SP++] = S_ISCHR(attr.buf->st_mode);
    return;
}

void c_isdir(instr_value *v)
{
    Stack[SP++] = S_ISDIR(attr.buf->st_mode);
    return;
}

void c_isfifo(instr_value *v)
{
    Stack[SP++] = S_ISFIFO(attr.buf->st_mode);
    return;
}

void c_islnk(instr_value *v)
{
    Stack[SP++] = S_ISLNK(attr.buf->st_mode);
    return;
}

void c_isreg(instr_value *v)
{
    Stack[SP++] = S_ISREG(attr.buf->st_mode);
    return;
}

void c_issock(instr_value *v)
{
    Stack[SP++] = S_ISSOCK(attr.buf->st_mode);
    return;
}

void c_mode(instr_value *v)
{
    Stack[SP++] = attr.buf->st_mode;
    return;
}

void c_mtime(instr_value *v)
{
    Stack[SP++] = attr.buf->st_mtime;
    return;
}

void c_nlink(instr_value *v)
{
    Stack[SP++] = attr.buf->st_nlink;
    return;
}

void c_rdev(instr_value *v)
{
    Stack[SP++] = attr.buf->st_rdev;
    return;
}

void c_blksize(instr_value *v)
{
    Stack[SP++] = attr.buf->st_blocks;
    return;
}

void c_size(instr_value *v)
{
    Stack[SP++] = attr.buf->st_size;
    return;
}

void c_uid(instr_value *v)
{
    Stack[SP++] = attr.buf->st_uid;
    return;
}

void c_nogroup(instr_value *v)
{
    Stack[SP++] = getgigid(attr.buf->st_gid) == (group_info *) NULL;
    return;
}

void c_nouser(instr_value *v)
{
    Stack[SP++] = getuiuid(attr.buf->st_uid) == (user_info *) NULL;
    return;
}

void c_depth(instr_value *v)
{
    Stack[SP++] = attr.depth;
    return;
}

void c_prune(instr_value *v)
{
    Stack[SP++] = 0;
    attr.prune = TRUE;
    return;
}


/* calculate the filename length */

void c_baselen(instr_value *v)
{
    char *	c;
    int	len;
    
    len = 0;
    for (c = attr.fname; *c != '\0'; c++) {
	if (*c == '/') {
	    len = 0;
	}
	else {
	    ++len;
	}
    }
    Stack[SP++] = len;
    return;
}


/* calculate the length of the directory portion of the path */

void c_dirlen(instr_value *v)
{
    char *	c;
    char *	slash = (char *) NULL;
    int	len;

    for (c = attr.fname; *c != '\0'; c++) {
	if (*c == '/') {
	    slash = c;
	}
    }
    if (slash == (char *) NULL) {
	len = 0;
    }
    else {
	len = (slash == attr.fname) ? 1 : (slash - attr.fname);
    }
    Stack[SP++] = len;
    return;
}


/* calculate length of the entire path */

void c_pathlen(instr_value *v)
{
    Stack[SP++] = strlen(attr.fname);
    return;
}


/*
 * c_fncase:
 *    This function does an exact file name match.
 */

void c_fncase(instr_value *v)
{
    char *	tail;
    
    if ((tail = strrchr(attr.fname, '/')) == (char *) NULL) {
	tail = attr.fname;
    }
    else {
	++tail;
    }

    Stack[SP++] = (strcmp(tail, v->pattern) == 0);

    return;
}

/*
 * c_fnigncase:
 *    This function does a case-insensitive file name match.
 */

void c_fnigncase(instr_value *v)
{
    char *	tail;
    
    if ((tail = strrchr(attr.fname, '/')) == (char *) NULL) {
	tail = attr.fname;
    }
    else {
	++tail;
    }

    Stack[SP++] = (strcasecmp(tail, v->pattern) == 0);

    return;
}

/*
 * c_pattern:
 *    This function implements the regular expression stuff.
 *    str is the the actual '\0' terminated string that occured in the
 *    expression (eg "*.BAK" ), minus the quotes "".
 */

void c_pattern(instr_value *v)
{
    extern int	wildmat(unsigned char *text, unsigned char *pattern,
			int dotspec, int igncase);

    char *	tail;

    
    if ((tail = strrchr(attr.fname, '/')) == (char *) NULL) {
	tail = attr.fname;
    }
    else {
	++tail;
    }

    Stack[SP++] = wildmat((unsigned char *) tail, (unsigned char *) v->pattern,
			  attr.dot_special, attr.ignore_case);

    return;
}

#if defined(sun)
void c_fstype(instr_value *v)
{
    fs_info *	p;

    Stack[SP] = 0;

    if (v->fstype != (char *) NULL) {
	if ((p = getfidev(attr.buf->st_dev)) != (fs_info *) NULL) {
	    if (strcmp(v->fstype, p->fs_type) == 0) {
		Stack[SP] = 1;
	    }
	}
    }
    SP++;
    return;
}
#else
void c_fstype(instr_value *v)
{
    Stack[SP++] = 0;
    return;
}
#endif

void c_samefs(instr_value *v)
{
    Stack[SP++] = (v->dev == (dev_t) 0) ? 1 : (attr.buf->st_dev == v->dev);
    return;
}

void c_syscall(instr_value *v)
{
    switch (v->sym->value) {

    case RH_SYSCALL_EXIT:
	exit(Stack[--SP]);

    default:
	error("%lu: unknown syscall", v->sym->value);
    }

    Stack[SP - v->sym->num_params] = Stack[SP - 1];
    SP -= v->sym->num_params - 1;

    return;
}

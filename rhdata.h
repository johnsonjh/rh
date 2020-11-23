/*
 * rhdata.h
 *
 * Written by:	Rick Ohnemus	(rick@sterling.com)
 *
 * rcsid: $Id: rhdata.h,v 1.1 2008/12/27 00:56:03 vandys Exp $
 */

#if !defined(__RHDATA_H__)
#define __RHDATA_H__

extern int	Issue_warnings;

extern int	Input_file;
extern char *	Input_expr_end;
extern char *	Input_expr_ptr;
extern char *	Input_name;

extern node *	Start_expr;

extern symbol *	Symbol_table;

extern instruction	StackProgram[];
extern int		PC;
extern int		startPC;

extern long		Stack[];
extern int		SP;
extern int		FP;

extern struct runtime	attr;

extern time_t		past_time;

extern symbol *		now;

#endif	/* __RHDATA_H__ */

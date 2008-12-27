/*
 * dump.h
 *
 * WRITTEN BY:	Rick Ohnemus	(rick@sterling.com)
 *
 * rcsid: $Id: dump.h,v 1.1 1994/01/12 19:34:18 rick Exp $
 */

#if !defined(__DUMP_H__)
#define __DUMP_H__

extern void	dump_instr(int pc, const instruction *ip);
extern int	dump_program(int pc, int flag);

#endif	/* __DUMP_H__ */

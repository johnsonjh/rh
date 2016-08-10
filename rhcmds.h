/*
 * rhcmds.h
 *
 * Written by:	Rick Ohnemus	(rick@sterling.com)
 *
 * rcsid: $Id: rhcmds.h,v 1.2 2010/04/22 03:03:16 vandys Exp $
 */

#if !defined(__RHCMDS_H__)
#define __RHCMDS_H__

extern void c_and(instr_value *);
extern void c_atime(instr_value *);
extern void c_band(instr_value *);
extern void c_baselen(instr_value *);
extern void c_bnot(instr_value *);
extern void c_bor(instr_value *);
extern void c_bxor(instr_value *);
extern void c_colon(instr_value *);
extern void c_ctime(instr_value *);
extern void c_date(instr_value *);
extern void c_depth(instr_value *);
extern void c_dev(instr_value *);
extern void c_dirlen(instr_value *);
extern void c_div(instr_value *);
extern void c_eq(instr_value *);
extern void c_fncase(instr_value *);
extern void c_fnigncase(instr_value *);
extern void c_func(instr_value *);
extern void c_fstype(instr_value *);
extern void c_ge(instr_value *);
extern void c_gid(instr_value *);
extern void c_gt(instr_value *);
extern void c_ino(instr_value *);
extern void c_isblk(instr_value *);
extern void c_ischr(instr_value *);
extern void c_isdir(instr_value *);
extern void c_isfifo(instr_value *);
extern void c_islnk(instr_value *);
extern void c_isreg(instr_value *);
extern void c_issock(instr_value *);
extern void c_le(instr_value *);
extern void c_lshift(instr_value *);
extern void c_lt(instr_value *);
extern void c_minus(instr_value *);
extern void c_mod(instr_value *);
extern void c_mode(instr_value *);
extern void c_mtime(instr_value *);
extern void c_mul(instr_value *);
extern void c_ne(instr_value *);
extern void c_nlink(instr_value *);
extern void c_nogroup(instr_value *);
extern void c_nop(instr_value *);
extern void c_not(instr_value *);
extern void c_nouser(instr_value *);
extern void c_number(instr_value *);
extern void c_or(instr_value *);
extern void c_param(instr_value *);
extern void c_pathlen(instr_value *);
extern void c_pattern(instr_value *);
extern void c_plus(instr_value *);
extern void c_prune(instr_value *);
extern void c_qm(instr_value *);
extern void c_rdev(instr_value *);
extern void c_return(instr_value *);
extern void c_rshift(instr_value *);
extern void c_samefs(instr_value *);
extern void c_size(instr_value *);
extern void c_blksize(instr_value *);
extern void c_syscall(instr_value *);
extern void c_uid(instr_value *);
extern void c_uniminus(instr_value *);

#endif	/* __RHCMDS_H__ */

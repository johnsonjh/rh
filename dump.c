/*
 * dump.c
 *
 * Written by:	Rick Ohnemus	(rick@sterling.com)
 *
 * This file contain data structures and functions to dump an rh
 * compiled program in a human readable form.
 *
 * Global Functions: dump_instr, dump_program
 */

#if !defined(lint)
static char rcsid[] = "$Id: dump.c,v 1.2 1994/01/26 01:46:21 rick Exp $";
#endif

#include <time.h>
#include "rh.h"
#include "rhcmds.h"
#include "rhdata.h"

extern int		activate(int pc);
extern void		deactivate(int pc);
extern const char *	func_name(int pc);

struct fmap {
    void (*func)(instr_value *);	/* function address */
    char *name;				/* function name */
};

/* map of rh C function addresses to names */

static const struct fmap funcinfo[] = {
    { c_and,		"c_and" },
    { c_atime,		"c_atime" },
    { c_band,		"c_band" },
    { c_baselen,	"c_baselen" },
    { c_bnot,		"c_bnot" },
    { c_bor,		"c_bor" },
    { c_bxor,		"c_bxor", },
    { c_colon,		"c_colon" },
    { c_ctime,		"c_ctime" },
    { c_date,		"c_date" },
    { c_depth,		"c_depth" },
    { c_dev,		"c_dev" },
    { c_dirlen,		"c_dirlen" },
    { c_div,		"c_div" },
    { c_eq,		"c_eq" },
    { c_fncase,		"c_fncase" },
    { c_fnigncase,	"c_fnigncase" },
    { c_fstype,		"c_fstype" },
    { c_func,		"c_func" },
    { c_ge,		"c_ge" },
    { c_gid,		"c_gid" },
    { c_gt,		"c_gt" },
    { c_ino,		"c_ino" },
    { c_isblk,		"c_isblk" },
    { c_ischr,		"c_ischr" },
    { c_isdir,		"c_isdir" },
    { c_isfifo,		"c_isfifo" },
    { c_islnk,		"c_islnk" },
    { c_isreg,		"c_isreg" },
    { c_issock,		"c_issock" },
    { c_le,		"c_le" },
    { c_lshift,		"c_lshift" },
    { c_lt,		"c_lt" },
    { c_minus,		"c_minus" },
    { c_mod,		"c_mod" },
    { c_mode,		"c_mode" },
    { c_mtime,		"c_mtime" },
    { c_mul,		"c_mul" },
    { c_ne,		"c_ne" },
    { c_nlink,		"c_nlink" },
    { c_nogroup,	"c_nogroup" },
    { c_nop,		"c_nop" },
    { c_not,		"c_not" },
    { c_nouser,		"c_nouser" },
    { c_number,		"c_number" },
    { c_or,		"c_or" },
    { c_param,		"c_param" },
    { c_pathlen,	"c_pathlen" },
    { c_pattern,	"c_pattern" },
    { c_plus,		"c_plus" },
    { c_prune,		"c_prune" },
    { c_qm,		"c_qm" },
    { c_rdev,		"c_rdev" },
    { c_return,		"c_return" },
    { c_rshift,		"c_rshift" },
    { c_samefs,		"c_samefs" },
    { c_size,		"c_size" },
    { c_syscall,	"c_syscall" },
    { c_uid,		"c_uid" },
    { c_uniminus,	"c_uniminus" },
    { NUM_PARAMS,	"num_params" },
    { NULL_INSTR,	NULL }
};

/*
 * dump_instr:
 *	Dump an rh instruction in a human readable form.
 *
 *	format of dump:
 *	    Calling system function
 *		[pc] C_func_name system_func_name (num_params)
 *	    Calling compiled function
 *		[pc] C_func_name RH_func_name [new PC] (num_params)
 *	    Date, file system type, or file name pattern matching
 *		[pc] C_func_name "pattern"
 *	    Return
 *		[pc] c_return (num_params)
 *	    Parameter
 *		[pc] c_param (parameter_number)
 *	    Typical instruction
 *		[pc] C_func_name value
 *	    Unknown C function address (this should never happen...)
 *		[pc] hex_address value
 *	    End of an rh function or program
 *		[pc] END
 */

void dump_instr(int pc, const instruction *ip)
{
    const struct fmap *	mp;

    for (mp = funcinfo; mp->func != NULL_INSTR; mp++) {
	if (mp->func == ip->func) {
	    if (ip->func == c_syscall) {
		(void) fprintf(stderr, "[%05d] %s %s {%d}\n",
			       pc, mp->name, ip->value.sym->name,
			       ip->value.sym->num_params);
	    }
	    else if (ip->func == c_func) {
		(void) fprintf(stderr, "[%05d] %s %s [%05d] (%d)\n",
			       pc, mp->name, func_name(ip->value.pc),
			       ip->value.pc,
			       StackProgram[ip->value.pc].value.num_params);
	    }
	    else if (ip->func == c_date) {
		(void) fprintf(stderr, "[%05d] %s \"%.24s\"\n",
			       pc, mp->name,
			       asctime(localtime(&ip->value.di->seconds)));
	    }
	    else if (ip->func == c_fstype) {
		(void) fprintf(stderr, "[%05d] %s \"%s\"\n",
			       pc, mp->name, ip->value.fstype);
	    }
	    else if (ip->func == c_pattern
		     || ip->func == c_fncase
		     || ip->func == c_fnigncase) {
		(void) fprintf(stderr, "[%05d] %s \"%s\"\n",
			       pc, mp->name, ip->value.pattern);
	    }
	    else if (ip->func == c_samefs) {
		(void) fprintf(stderr, "[%05d] %s %04x %04x\n",
			       pc, mp->name,
			       (u_int) major(ip->value.dev),
			       (u_int) minor(ip->value.dev));
	    }
	    else if (ip->func == c_return) {
		(void) fprintf(stderr, "[%05d] %s (%d)\n",
			       pc, mp->name, ip->value.num_params);
	    }
	    else if (ip->func == c_param) {
		(void) fprintf(stderr, "[%05d] %s (%d)\n",
			       pc, mp->name, ip->value.param);
	    }
	    else {
		(void) fprintf(stderr, "[%05d] %s %ld\n",
			       pc, mp->name, ip->value.l);
	    }
	    break;
	}
    }

    if (mp->func == NULL_INSTR) {
	/* Something is wrong if this gets executed! */
	(void) fprintf(stderr, "[%05d] %p %ld\n", pc, ip->func, ip->value.l);
    }

    return;
}

/*
 * dump_program:
 *	Dump an rh compiled program in a human readable	form.
 *
 *	If 'flag' is true then rh function calls will be followed.
 */

int dump_program(int pc, int flag)
{
    const char *	fn;	/* compiled function name */
    const instruction *	ip;	/* pointer to current instruction */
    
    if (StackProgram[pc].func == END_OF_PROG) {
	return -1;
    }

    /* display function name if at beginning of compiled function */

    if ((fn = func_name(pc)) != NULL) {
	(void) fprintf(stderr, "  function %s\n", fn);
    }

    for (ip = &StackProgram[pc]; ip->func != NULL_INSTR; ip++, pc++) {
	dump_instr(pc, ip);
	if (flag && ip->func == c_func) {
	    if (activate(ip->value.pc)) {
		(void) dump_program(ip->value.pc, flag);
		deactivate(ip->value.pc);
	    }
	}
    }

    (void) fprintf(stderr, "[%05d] END\n", pc);

    return pc + 1;
}    

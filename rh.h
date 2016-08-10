/*
 * rh.h
 *
 * Written by:	Ken Stauffer
 *		Rick Ohnemus	(rick@sterling.com)
 *
 * rcsid: $Id: rh.h,v 1.1 2008/12/27 00:56:03 vandys Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <unistd.h>


#if !defined(FALSE)
#  define FALSE	0
#endif

#if !defined(TRUE)
#  define TRUE	1
#endif


#define RHRC		".rhrc"		/* start up file */
#define RHRC_ENV	"RHRC"		/* rh environment variable */
#define RHPATH_ENV	"RHPATH"	/* paths to search for program files */
#define HOME_ENV	"HOME"		/* path to your home directory */

#if !defined(MAX_PROG_LEN)
#  define MAX_PROG_LEN	2000	/* size of stack program */
#endif

#if !defined(MAX_STACK_LEN)
#  define MAX_STACK_LEN	1000	/* size of stack */
#endif

#define NULL_INSTR	((void (*)(instr_value *)) NULL)
#define END_OF_PROG	((void (*)(instr_value *)) -1)
#define EXPR_LIST	((void (*)(instr_value *)) -2)
#define TERTIARY	((void (*)(instr_value *)) -3)
#define UNRES_FUNC	((void (*)(instr_value *)) -4)
#define NUM_PARAMS	((void (*)(instr_value *)) -5)
#define UNUSED_FUNC	((void (*)(instr_value *)) -6)

/* The '- 3' in DEPTH defines is for stdin, stdout, and stderr */

#if defined(BSD)
#  if defined(SUNOS_4)
#    define POSIX_DIRECTORY_LIBRARY
#    include <sys/time.h>
#    include <sys/resource.h>
#    if defined(RLIMIT_NOFILE)
#      define DEPTH	(max_descriptors() - 3)
#    else
#      define DEPTH	(getdtablesize() - 3)
#    endif /* RLIMIT_NOFILE */
#  else
#    define DEPTH	(getdtablesize() - 3)
#  endif /* SUNOS_4 */
#endif /* BSD */

#if defined(SYSV)
#  if defined(SYSVR3)
#    define DEPTH	(ulimit(4, 0L) - 3)
#    define POSIX_DIRECTORY_LIBRARY
#  else /* SYSVR3 */
     /* This value was arbitrarily chosen */
#    define DEPTH	(20 - 3)
#  endif /* SYSVR3 */
#endif /* SYSV */


/*
 * XXX - on BSD systems, this is defined in <sys/param.h>.
 * On System V Release 3, it's defined inside "nami.c", and is,
 * unfortunately, not in any include file.
 * On systems other than those, no such simple limit exists.
 * On BSD and S5R3 systems, as distributed by Berkeley and AT&T,
 * respectively, it's 1024, so we set it to that.
 */

#if !defined(MAXPATHLEN)
#  if defined(PATH_MAX)
#    define MAXPATHLEN	PATH_MAX
#  else
#    if defined(MAXNAMLEN)
#      define MAXPATHLEN  MAXNAMLEN
#    else
#      define MAXPATHLEN  1024
#    endif
#  endif
#endif

#if !defined(MAXNAMLEN)
#  if defined(PATH_MAX)
#    define MAXNAMLEN	PATH_MAX
#  else
#    define MAXNAMLEN	255
#  endif
#endif


#include <string.h>
#include "getdate.h"


/*
 * Some System do not define these macro's.
 */

#if !defined(major)
#  define major(b)	((((unsigned) (b)) >> 8) & 0xff)
#endif

#if !defined(minor)
#  define minor(b)	(((unsigned) (b)) & 0xff)
#endif

#if !defined(S_ISBLK)
#  define S_ISBLK(m)	(((m) & S_IFMT) == S_IFBLK)
#endif

#if !defined(S_ISCHR)
#  define S_ISCHR(m)	(((m) & S_IFMT) == S_IFCHR)
#endif

#if !defined(S_ISDIR)
#  define S_ISDIR(m)	(((m) & S_IFMT) == S_IFDIR)
#endif

#if defined(S_IFIFO)
#  if !defined(S_ISFIFO)
#    define S_ISFIFO(m)	(((m) & S_IFMT) == S_IFIFO)
#  endif
#else
#  define S_ISFIFO(m)	(0)
#endif

#if defined(S_IFLNK)
#  if !defined(S_ISLNK)
#    define S_ISLNK(m)	(((m) & S_IFMT) == S_IFLNK)
#  endif
#else
#  define S_ISLNK(m)	(0)
#endif

#if !defined(S_ISREG)
#  define S_ISREG(m)	(((m) & S_IFMT) == S_IFREG)
#endif

#if defined(S_IFSOCK)
#  if !defined(S_ISSOCK)
#    define S_ISSOCK(m)	(((m) & S_IFMT) == S_IFSOCK)
#  endif
#else
#  define S_ISSOCK(m)	(0)
#endif


/*
 * Builtin system calls
 */

#define RH_SYSCALL_EXIT		0


typedef struct _instruction	instruction;
typedef struct _node		node;
typedef struct _symbol		symbol;


/*
 * Structure of a "rh-assembly" instruction.
 */

typedef union {
    dev_t	dev;
    date_info *	di;
    char *	fstype;
    char *	func_name;
    long	l;
    int		num_params;
    int		param;
    char *	pattern;
    int		pc;
    symbol *	sym;
    node *	tree;
} instr_value;

struct _instruction {
   void	      (*func)(instr_value *);
   instr_value	value;
};


/*
 * Structure of a parse tree node
 */

struct _node {
    instruction	code;
    node *	left;
    node *	right;
    node *	middle;
    int		count;
    node **	trees;
};


/*
 * Structure of a symbol.
 */

struct _symbol {
   char *	name;		/* name */
   short	type;		/* type */
   short	req_stat;	/* requires stat() call */
   long		value;		/* value */
   void	      (*func)(instr_value *);	/* C function to execute */
   symbol *	next;		/* next symbol in list */
   short	ref_count;      /* func. parameter reference count */
   short	num_params;	/* # of params an rh function has */
   node *	tree;		/* parse tree for an rh function */
};



/*
 * Structure defining the rh runtime environment.
 */

struct runtime {
   struct stat *	buf;	/* stat info of current file */
   char *		fname;	/* file name of current file */
   int			depth;	/* relative depth of current file */
   void		      (*func)(void); /* examination function */
   char *		command; /* command to exec for current file */
   char *		format;	/* format for printing file information */
   int			num_errors; /* number of errors */
   /* ---------- flags ---------- */
   char			dot_special;	/* dot at start of filename special? */
#if defined(S_IFLNK)
   char			follow_dir;	/* follow directory symlinks */
   char			follow_file;	/* follow non-directory symlinks */
#endif
   char			graphic;	/* display non-graphic characters in filenames as ? */
   char			ignore_case;	/* ignore case when comparing filenames */
   char			prune;		/* flag to indicate prunning */
   char			stat_all;	/* stat() all files */
   char			str_owner;	/* print user/group strings instead of numbers */
   char			verbose;	/* used by the (*func)() routine */
   char			full_date;	/* print full date instead of ls(1) type date */
};

/*
 * rhdata.c
 *
 * Written by:	Ken Stauffer
 *		Rick Ohnemus	(rick@sterling.com)
 *
 * Global Functions: rhfinish, rhinit
 */

#if !defined(lint)
static char rcsid[] = "$Id: rhdata.c,v 1.2 2010/04/22 03:03:16 vandys Exp $";
#endif

#include "rh.h"
#include "rhcmds.h"
#include "rhgram.h"
#include "errmsg.h"

#if !defined(S_IREAD)
#  define S_IREAD	0400
#  define S_IWRITE	0200
#  define S_IEXEC	0100
#endif

#if !defined(S_IRWXU)
#  define S_IRWXU	0700
#  define S_IRWXG	0070
#  define S_IRWXO	0007
#endif

#if !defined(S_IRUSR)
#  define S_IRGRP	0040
#  define S_IROTH	0004
#  define S_IRUSR	0400
#  define S_IWGRP	0020
#  define S_IWOTH	0002
#  define S_IWUSR	0200
#  define S_IXGRP	0010
#  define S_IXOTH	0001
#  define S_IXUSR	0100
#endif

int Issue_warnings = FALSE;

int Input_file;
char *Input_expr_end;
char *Input_expr_ptr;
char *Input_name;

node *Start_expr = (node *) NULL;

symbol *Symbol_table;

instruction StackProgram[MAX_PROG_LEN];
int PC;
int startPC;

long Stack[MAX_STACK_LEN + 3];
int SP;
int FP;

struct runtime attr;

/*
 * NOW - 6 months (actually just 180 days)
 */

long past_time;

/*
 * symbol table entry for NOW
 */

symbol *now;

/*
 * Do not move the location of NOW in the following table
 * without changing the code in rhinit() that sets 'now'.
 */

static symbol init_syms[] = {
    { "NOW",      NUMBER,      FALSE,  0,                 c_number },
    { "IEXEC",    NUMBER,      FALSE,  S_IEXEC,           c_number },
    { "IFBLK",    NUMBER,      FALSE,  S_IFBLK,           c_number },
    { "IFCHR",    NUMBER,      FALSE,  S_IFCHR,           c_number },
    { "IFDIR",    NUMBER,      FALSE,  S_IFDIR,           c_number },
#if defined(S_IFIFO)
    { "IFIFO",    NUMBER,      FALSE,  S_IFIFO,           c_number },
#endif
#if defined(S_IFLNK)
    { "IFLNK",    NUMBER,      FALSE,  S_IFLNK,           c_number },
#endif
    { "IFMT",     NUMBER,      FALSE,  S_IFMT,            c_number },
    { "IFREG",    NUMBER,      FALSE,  S_IFREG,           c_number },
#if defined(S_IFSOCK)
    { "IFSOCK",   NUMBER,      FALSE,  S_IFSOCK,          c_number },
#endif
    { "IREAD",    NUMBER,      FALSE,  S_IREAD,           c_number },
    { "IRGRP",    NUMBER,      FALSE,  S_IRGRP,           c_number },
    { "IROTH",    NUMBER,      FALSE,  S_IROTH,           c_number },
    { "IRUSR",    NUMBER,      FALSE,  S_IRUSR,           c_number },
    { "IRWXG",    NUMBER,      FALSE,  S_IRWXG,           c_number },
    { "IRWXO",    NUMBER,      FALSE,  S_IRWXO,           c_number },
    { "IRWXU",    NUMBER,      FALSE,  S_IRWXU,           c_number },
    { "ISGID",    NUMBER,      FALSE,  S_ISGID,           c_number },
    { "ISUID",    NUMBER,      FALSE,  S_ISUID,           c_number },
    { "ISVTX",    NUMBER,      FALSE,  S_ISVTX,           c_number },
    { "IWGRP",    NUMBER,      FALSE,  S_IWGRP,           c_number },
    { "IWOTH",    NUMBER,      FALSE,  S_IWOTH,           c_number },
    { "IWRITE",   NUMBER,      FALSE,  S_IWRITE,          c_number },
    { "IWUSR",    NUMBER,      FALSE,  S_IWUSR,           c_number },
    { "IXGRP",    NUMBER,      FALSE,  S_IXGRP,           c_number },
    { "IXOTH",    NUMBER,      FALSE,  S_IXOTH,           c_number },
    { "IXUSR",    NUMBER,      FALSE,  S_IXUSR,           c_number },
    { "atime",    TIME_FIELD,  TRUE,   0,                 c_atime },
    { "baselen",  BLTIN,       FALSE,  0,                 c_baselen },
    { "blocks",   FIELD,       TRUE,   0,                 c_blksize },
    { "ctime",    TIME_FIELD,  TRUE,   0,                 c_ctime },
    { "days",     NUMBER,      FALSE,  24 * 60 * 60,      c_number },
    { "depth",    BLTIN,       FALSE,  0,                 c_depth },
    { "dev",      FIELD,       TRUE,   0,                 c_dev },
    { "dirlen",   BLTIN,       FALSE,  0,                 c_dirlen },
    { "gid",      FIELD,       TRUE,   0,                 c_gid },
    { "group",    FIELD,       TRUE,   0,                 c_gid },
    { "hours",    NUMBER,      FALSE,  60 * 60,           c_number },
    { "ino",      FIELD,       TRUE,   0,                 c_ino },
    { "isblk",    BLTIN,       TRUE,   0,                 c_isblk },
    { "ischr",    BLTIN,       TRUE,   0,                 c_ischr },
    { "isdir",    BLTIN,       TRUE,   0,                 c_isdir },
#if defined(S_IFIFO)
    { "isfifo",   BLTIN,       TRUE,   0,                 c_isfifo },
#endif
#if defined(S_IFLNK)
    { "islnk",    BLTIN,       TRUE,   0,                 c_islnk },
#endif
    { "isreg",    BLTIN,       TRUE,   0,                 c_isreg },
#if defined(S_IFSOCK)
    { "issock",   BLTIN,       TRUE,   0,                 c_issock },
#endif
    { "mode",     FIELD,       TRUE,   0,                 c_mode },
    { "mtime",    TIME_FIELD,  TRUE,   0,                 c_mtime },
    { "nlink",    FIELD,       TRUE,   0,                 c_nlink },
    { "nogroup",  BLTIN,       TRUE,   0,                 c_nogroup },
    { "nouser",   BLTIN,       TRUE,   0,                 c_nouser },
    { "owner",    FIELD,       TRUE,   0,                 c_uid },
    { "pathlen",  BLTIN,       FALSE,  0,                 c_pathlen },
    { "prune",    BLTIN,       FALSE,  0,                 c_prune },
    { "rdev",     FIELD,       TRUE,   0,                 c_rdev },
    { "return",   RETURN,      FALSE,  0,                 c_return },
    { "size",     FIELD,       TRUE,   0,                 c_size },
    { "strlen",   BLTIN,       FALSE,  0,                 c_baselen },
    { "uid",      FIELD,       TRUE,   0,                 c_uid },
    { "user",     FIELD,       TRUE,   0,                 c_uid },
    { "weeks",    NUMBER,      FALSE,  24 * 60 * 60 * 7,  c_number },
    { "exit",	  SYSTEM_CALL, FALSE,  RH_SYSCALL_EXIT,	  c_syscall,
	  (symbol *) NULL, 0, 1 }
};

void rhinit(void)
{
    extern time_t time();
    
    int i;
    
    
    Symbol_table = &init_syms[0];
    
    for (i = 0; i < ((sizeof(init_syms) / sizeof(symbol)) - 1); i++) {
        init_syms[i].next = &init_syms[i + 1];
    }
    init_syms[i].next = (symbol *) NULL;
    
    /*
     * get symbol table entry for NOW variable
     * 		could use this instead:  now = locatename("NOW");
     */

    now = &init_syms[0];

    /* Initialize the NOW variable to the time right now */

    now->value = (long) time((time_t *) 0);
    
    past_time = now->value - (6L * 30L * 24L * 60L * 60L);
    
    return;
}

void rhfinish(void)
{
    symbol *	s;

    extern void free_tree(node *);

    
#if defined(DEBUG)
    for (s = Symbol_table; s != (symbol *) NULL; s = s->next) {
        if (s->type == PARAM) {
            error("symbol table is corrupt!");
        }
        else if (s->type != FUNCTION) {
            break;
        }
        else {
            free_tree(s->tree);
            s->tree = (node *) NULL;
        }
    }
#else
    while (Symbol_table->type == PARAM || Symbol_table->type == FUNCTION) {
        if (Symbol_table->type == PARAM) {
            error("symbol table is corrupt!");
        }
        s = Symbol_table;
        Symbol_table = Symbol_table->next;
        free_tree(s->tree);
        free(s->name);
        free((char *) s);
    }
#endif
    
    return;
}

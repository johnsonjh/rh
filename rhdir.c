/*
 * rhdir.c
 *
 * Written by:	Ken Stauffer
 *		Rick Ohnemus	(rick@sterling.com)
 * 
 * This file contains the "non portable" stuff dealing with
 * directories.
 *
 * Global Functions: ftrw, graphic, printentry, printformat
 *
 * Static Functions: fwt1, handle_file, ls_date
 */
#include <ctype.h>
#include "rh.h"
#include "rhdata.h"
#include "errmsg.h"
#include "sysinfo.h"

#define user_index(b)	((000777 & (b)) >> 6) + ((b) & S_ISUID ? 8 : 0) 
#define group_index(b)	((000077 & b) >> 3) + ((b) & S_ISGID ? 8 : 0)
#define all_index(b)	((000007 & (b)) + (((b) & S_ISVTX) ? 8 : 0))
#define ftype_index(b)	((b) >> 13)

/*
 * This is pretty questionable; in a traditional filesystem you get
 *  a link for "." and for "..", thus > 2 means you have other directories
 *  which link to you.  But in virtual (fuse) filesystems, not so much.
 *  sshfs seems to just return 1 no matter what.
 */
#define hassubdir(b)	((b)->st_nlink != 2)

#define isdot(s)	((s)[1] == '\0' && (s)[0] == '.')
#define isdotdot(s)	((s)[2] == '\0' && (s)[1] == '.' && (s)[0] == '.')

#if !defined(S_IFLNK)
#  define    lstat        stat
#endif

#define isproper(m)    (!attr.prune && S_ISDIR(m))

#if defined(POSIX_DIRECTORY_LIBRARY)
#  include <dirent.h>
#else
#  include <sys/dir.h>
#endif

extern char *ctime();


static char *ftype[] = { "p", "c" , "d" , "b" , "-" , "l" , "s" , "t" };

static char *perm[] = {
    "---", "--x", "-w-", "-wx" , "r--", "r-x", "rw-", "rwx" ,
    "--S", "--s", "-wS", "-ws" , "r-S", "r-s", "rwS", "rws"
};

static char *perm2[] = {
    "---", "--x", "-w-", "-wx" , "r--", "r-x", "rw-", "rwx" ,
    "--T", "--t", "-wT", "-wt" , "r-T", "r-t", "rwT", "rwt"
};

#if defined(DEBUG)
extern int	Trace;
#endif

/*
 * graphic:
 *	Returns copy of input string with all non-graphics characters
 *	replaced with '?'.
 */

char *graphic(const char *name)
{
    static char new_name[MAXPATHLEN + 1];
    
    char c, *p = new_name;
    
    while ((c = *name++) != '\0') {
	if ((c == ' ') || (isascii(c) && isgraph(c))) {
	    *p++ = c;
	} else {
	    *p++ = '?';
	}
    }
    *p = '\0';
    return new_name;
}

/*
 * ls_date:
 *	Returns a time converted to a string that may resemble
 *	what ls(1) outputs.
 *
 *	If full_date is	true then the time string from ctime
 *	is copied and returned.
 *	If full_date is false and the time is more than 180 days
 *	in the past or more than 5 minutes into the future then
 *	the month, day,and year are returned.
 *	Otherwise, return the month, day, time, and year.
 */

static char *ls_date(time_t ftime,
		     int full_date)
{
#if SUNOS_4
    char *ts = ctime(&ftime);
#else
    char *ts = ctime((long *) &ftime);
#endif
    static char date[32];
    
    if (full_date) {
	char *to = date;
	while (*ts != '\n' && *ts != '\0') {
	    *to++ = *ts++;
	}
	*to = '\0';
    }
    else if (ftime < past_time || ftime > (now->value + 5L * 60L)) {
	(void) sprintf(date, "%6.6s  %4.4s", ts + 4, ts + 20);
    }
    else {
	(void) sprintf(date, "%12.12s", ts + 4);
    }
    
    return date;
}

/*
 * printformat:
 *	Outputs file information using the format specified
 *	using the '-p' switch.
 */

void printformat(struct stat *buf,
		 const char *name,
		 int depth)
{
    static char *hex_digits = "0123456789abcdef";

    const char *cp;
    unsigned char c;
    char *fp, *p;
    group_info *gi;
    int	i;
    user_info *ui;

    for (fp = attr.format; *fp != '\0'; fp++) {
	if (*fp != '%') {
	    if (*fp != '\\') {
		(void) putc(*fp, stdout);
		continue;
	    }
	    fp++;
	    
	    switch (*fp) {
		
	    case '\0':
		(void) putc('\\', stdout);
		break;
		
	    case '0':
		if (*(fp + 1) == 'x' || *(fp + 1) == 'X') {
		    if (isxdigit(*(fp + 2))) {
			fp += 2;
			for (i = c = 0; i < 2 && isxdigit(*fp); i++, fp++) {
			    c <<= 4;
			    if (isupper(*fp))
				*fp = tolower(*fp);
			    c += strchr(hex_digits, *fp) - hex_digits;
			}
			(void) putc((char) c, stdout);
			fp--;
		    }
		    else
			(void) putc(*fp, stdout);
		}
		else if (*(fp + 1) >= '0' && *(fp + 1) <= '7') {
		    fp++;
		    for (i = c = 0;
			 i < 3 && *fp >= '0' && *fp <= '7';
			 i++, fp++) {
			c <<= 3;
			c += *fp - '0';
		    }
		    (void) putc((char) c, stdout);
		    fp--;
		}
		else
		    (void) putc(*fp, stdout);
		break;
		
	    case 'a':
		(void) putc('\007', stdout);
		break;
		
	    case 'b':
		(void) putc('\b', stdout);
		break;
		
	    case 'f':
		(void) putc('\f', stdout);
		break;
		
	    case 'n':
		(void) putc('\n', stdout);
		break;
		
	    case 'r':
		(void) putc('\r', stdout);
		break;
		
	    case 't':
		(void) putc('\t', stdout);
		break;
		
	    case 'v':
		(void) putc('\v', stdout);
		break;
		
	    default:
		(void) putc(*fp, stdout);
		break;
	    }
	    
	    continue;
	}
	
	fp++;
	
	switch (*fp) {
	    
	case '\0':
	    error("format character missing after '%%'");
	    /*NOTREACHED*/
	    
	case 'a':	/* atime as ls date */
	    (void) printf("%s", ls_date(buf->st_atime, attr.full_date));
	    break;
	    
	case 'A':	/* atime in seconds */
	    (void) printf("%10lu", (u_long) buf->st_atime);
	    break;

#if BSD
	case 'b':	/* number blocks allocated to file */
	    (void) printf("%10lld", buf->st_blocks);
	    break;
	    
	case 'B':	/* optimal block size for file system i/o */
	    (void) printf("%6ld", buf->st_blksize);
	    break;
#endif
	    
	case 'c':	/* ctime as ls date */
	    (void) printf("%s", ls_date(buf->st_ctime, attr.full_date));
	    break;
	    
	case 'C':	/* ctime in seconds */
	    (void) printf("%10lu", (u_long) buf->st_ctime);
	    break;

	case 'd':	/* parent directory */
	    if ((p = strrchr(name, '/')) == (char *) NULL)
		(void) putc('.', stdout);
	    else if (p == name)
		(void) putc('/', stdout);
	    else {
		*p = '\0';
		(void) printf("%s", attr.graphic ? graphic(name) : name);
		*p = '/';
	    }
	    break;
	    
	case 'D':	/* depth of file relative to search start point */
	    (void) printf("%4d", depth);
	    break;
	    
	case 'g':	/* numeric group id  */
	    (void) printf("%8d", buf->st_gid);
	    break;
	    
	case 'G':	/* symbolic group id */
	    if ((gi = getgigid(buf->st_gid)) == (group_info *) NULL) {
		(void) printf("%8d", buf->st_gid);
	    }
	    else {
		(void) printf("%-8.8s", gi->name);
	    }
	    break;
	    
	case 'i':	/* inode number */
	    (void) printf("%6llu", buf->st_ino);
	    break;
	    
	case 'l':	/* number of links */
	    (void) printf("%3d", buf->st_nlink);
	    break;
	    
	case 'm':	/* mtime as ls date */
	    (void) printf("%s", ls_date(buf->st_mtime, attr.full_date));
	    break;
	    
	case 'M':	/* mtime in seconds */
	    (void) printf("%10lu", (u_long) buf->st_mtime);
	    break;

	case 'p':	/* permissions bits in ls format */
	    (void) printf("%s%s%s%s", 
			  ftype[ftype_index(buf->st_mode)],
			  perm[user_index(buf->st_mode)],
			  perm[group_index(buf->st_mode)],
			  perm2[all_index(buf->st_mode)]);
	    break;
	    
	case 'P':	/* numeric permission bits */
	    (void) printf("%#6o", (u_int) buf->st_mode);
	    break;
	    
	case 'r':	/* minor device if device file */
	    if (S_ISCHR(buf->st_mode) || S_ISBLK(buf->st_mode))
		(void) printf("%4u", (u_int) minor(buf->st_rdev));
	    else
		(void) printf("xxxx");
	    break;
	    
	case 'R':	/* major device if device file */
	    if (S_ISCHR(buf->st_mode) || S_ISBLK(buf->st_mode))
		(void) printf("%4u", (u_int) major(buf->st_rdev));
	    else
		(void) printf("xxxx");
	    break;
	    
	case 'n':	/* full path of file (relative to start point) */
	case 's':
	    (void) printf("%s", attr.graphic ? graphic(name) : name);
	    break;
	    
	case 'N':	/* base name of file */
	case 'S':
	    cp = strrchr(name, '/');
	    if (cp == NULL) {
		cp = name;
	    }
	    (void)printf("%s", attr.graphic ? graphic(cp) : cp);
	    break;
	    
	case 'u':	/* numeric user id */
	    (void) printf("%8d", buf->st_uid);
	    break;
	    
	case 'U':	/* symbolic user id */
	    if ((ui = getuiuid(buf->st_uid)) == (user_info *) NULL) {
		(void) printf("%8d", buf->st_uid);
	    }
	    else {
		(void) printf("%-8.8s", ui->name);
	    }
	    break;
	    
	case 'w':	/* minor device of device where file resides */
	    (void) printf("%02x", (u_int) minor(buf->st_dev));
	    break;
	    
	case 'W':	/* major device of device where file resides */
	    (void) printf("%02x", (u_int) major(buf->st_dev));
	    break;
	    
	case 'y':	/* numeric device of device where file resides */
	    (void) printf("%04x", (u_int) buf->st_dev);
	    break;
	    
	case 'z':	/* file size in bytes */
	    (void) printf("%9lld", buf->st_size);
	    break;
	    
	default:
	    (void) putc(*fp, stdout);
	    break;
	}
    }
    
    (void) putc('\n', stdout);
    return;
}

/*
 * printentry:
 *	Display filename,permissions and size in a '/bin/ls' like
 *	format. If verbose is non-zero then more information is
 *	displayed.
 * uses the macros:
 *	user_index(b)
 *	group_index(b)
 *	all_index(b)
 *	ftype_index(b)
 *
 */

void printentry(int verbose,
		int str_owner,
		int full_date,
		struct stat *buf,
		const char *name)
{
    group_info *	gi;
    char		gid_buf[20];
    user_info *		ui;
    char		uid_buf[20];
    
    
    if (verbose) {
	if (str_owner) {
	    if ((ui = getuiuid(buf->st_uid)) == (user_info *) NULL) {
		(void) sprintf(uid_buf, "%8d", buf->st_uid);
	    }
	    else {
		(void) sprintf(uid_buf, "%-8.8s", ui->name);
	    }
	    if ((gi = getgigid(buf->st_gid)) == (group_info *) NULL){ 
		(void) sprintf(gid_buf, "%8d", buf->st_gid);
	    }
	    else {
		(void) sprintf(gid_buf, "%-8.8s", gi->name);
	    }
	}
	else {
	    (void) sprintf(uid_buf, "%8d", buf->st_uid);
	    (void) sprintf(gid_buf, "%8d", buf->st_gid);
	}
	if (S_ISCHR(buf->st_mode) || S_ISBLK(buf->st_mode)) {
	    (void) printf("%s%s%s%s %s %s %3d,%3d   %s %s\n",
			  ftype[ftype_index(buf->st_mode)],
			  perm[user_index(buf->st_mode)],
			  perm[group_index(buf->st_mode)],
			  perm2[all_index(buf->st_mode)],
			  uid_buf,
			  gid_buf,
			  major(buf->st_rdev),
			  minor(buf->st_rdev),
			  ls_date(buf->st_mtime, full_date),
			  attr.graphic ? graphic(name) : name);
	}
	else {
	    (void) printf("%s%s%s%s %s %s %9lld %s %s\n",
			  ftype[ftype_index(buf->st_mode)],
			  perm[user_index(buf->st_mode)],
			  perm[group_index(buf->st_mode)],
			  perm2[all_index(buf->st_mode)],
			  uid_buf,
			  gid_buf,
			  buf->st_size,
			  ls_date(buf->st_mtime, full_date),
			  attr.graphic ? graphic(name) : name);
	}
    }
    else {
	if (S_ISCHR(buf->st_mode) || S_ISBLK(buf->st_mode)) {
	    (void) printf("%s%s%s%s %3d,%3d   %s\n",
			  ftype[ftype_index(buf->st_mode)],
			  perm[user_index(buf->st_mode)],
			  perm[group_index(buf->st_mode)],
			  perm2[all_index(buf->st_mode)],
			  major(buf->st_rdev),
			  minor(buf->st_rdev),
			  attr.graphic ? graphic(name) : name);
	}
	else {
	    (void) printf("%s%s%s%s %9lld %s\n",
			  ftype[ftype_index(buf->st_mode)],
			  perm[user_index(buf->st_mode)],
			  perm[group_index(buf->st_mode)],
			  perm2[all_index(buf->st_mode)],
			  buf->st_size,
			  attr.graphic ? graphic(name) : name);
	}
    }
    
    return;
}

/*
 * handle_file:
 *	If symbolic links:
 *		lstat() file
 *		if failure then print warning and return
 *		if success and file is a link then
 *			stat() the file
 *			if directory and following directory links
 *				update file name with link value
 *				update stat buffer if not link to current dir.
 *			if regular file and following file links
 *				update stat buffer
 *		call examination function to process file
 *			
 *
 *	If no symbolic links:	(life is much simpler)
 *		stat() file
 *		if failure then print warning and return
 *		if success then call examination function to process file
 */

static int handle_file(void)
{
    struct stat	statbuf;

#if defined(DEBUG)
    if (Trace) {
	(void) fprintf(stderr, "handle_file(\"%s\")\n", attr.fname);
    }
#endif

#if defined(S_IFLNK)
    if (lstat(attr.fname, attr.buf) < 0) {
	warning("%s: %m", attr.fname);
	attr.num_errors++;
	return 0;
    }
    if ((attr.follow_dir || attr.follow_file) && S_ISLNK(attr.buf->st_mode)) {
	if (stat(attr.fname, &statbuf) == 0) {
	    if (S_ISDIR(statbuf.st_mode)) {
		if (attr.follow_dir) {
		    int   len;
		    char  linkbuf[MAXPATHLEN];

		    len = readlink(attr.fname, linkbuf, sizeof(linkbuf));
		    if (len != 1 || *linkbuf != '.') {
			*attr.buf = statbuf;
		    }
		}
	    }
	    else if (attr.follow_file) {
		*attr.buf = statbuf;
	    }
	}
	else {
	    attr.num_errors++;
	}
    }
#else
    if (stat(attr.fname, attr.buf) < 0) {
	warning("%s: %m", attr.fname);
	attr.num_errors++;
	return 0;
    }
#endif

    (*attr.func)();
    return 1;
}

/*
 * fwt1:
 *	'p' points to the end of the string in attr.fname
 *
 *	2 versions of this routine currently live here:
 *	"new-style", for systems with a BSD or POSIX-style
 *	directory library, and systems without such a
 *	directory library. They both differ in
 *	the manner in which they access directories.
 *	Any changes needed to work on another system
 *	should only have to made for this routine.
 *
 *	Below is the "directory library" version of fwt1()
 *
 */

#if defined(POSIX_DIRECTORY_LIBRARY) || defined(BSD)

static void fwt1(int depth,
		 int dashr,
		 char *p,
		 int stat_all,
		 int nlink)
{
#if defined(POSIX_DIRECTORY_LIBRARY)
    struct dirent *	dp;
#  define DIRTYPE   struct dirent
#else
    struct direct *	dp;
#  define DIRTYPE  struct direct
#endif

    DIR *		dirp;
    char *		q;
    char *		s;

    
    if (depth == 0) {
	if (dashr) {
	    warning("%s: file tree depth exceeded", attr.fname);
	}
	return;
    }
    attr.depth++;
    
    q = attr.fname;
    if (!*q) {
	q = ".";
    }
    if ((dirp = opendir(q)) == (DIR *) NULL) {
	warning("%s: %m", attr.fname);
	return;
    }
    
    nlink -= 2;

    if (stat_all) {
	if (attr.stat_all) {
	    while ((dp = readdir(dirp)) != (DIRTYPE *) NULL) {
		if (isdot(dp->d_name) || isdotdot(dp->d_name)) {
		    continue;
		}
		s = p;
		q = dp->d_name;
		while ((*s++ = *q++) != '\0') {
		    ;		/* empty loop */
		}
		s--;

		attr.prune = FALSE;
		if (!handle_file()) {
		    continue;
		}
		if (isproper(attr.buf->st_mode)) {
		    *s++ = '/';
		    *s = '\0';
		    fwt1(depth - 1, dashr, s,
			 attr.stat_all | hassubdir(attr.buf),
			 attr.buf->st_nlink);
		}
	    }
	}
	else {
	    while ((dp = readdir(dirp)) != (DIRTYPE *) NULL) {
		if (isdot(dp->d_name) || isdotdot(dp->d_name)) {
		    continue;
		}
		s = p;
		q = dp->d_name;
		while ((*s++ = *q++) != '\0') {
		    ;		/* empty loop */
		}
		s--;
		
		attr.prune = FALSE;
		if (!handle_file()) {
		    continue;
		}
		if (isproper(attr.buf->st_mode)) {
		    *s++ = '/';
		    *s = '\0';
		    fwt1(depth - 1, dashr, s,
			 attr.stat_all | hassubdir(attr.buf),
			 attr.buf->st_nlink);
		    nlink--;
		    if (nlink == 0) {
			break;
		    }
		}
	    }
	    if (dp != (DIRTYPE *) NULL) {
		while ((dp = readdir(dirp)) != (DIRTYPE *) NULL) {
		    if (isdot(dp->d_name) || isdotdot(dp->d_name)) {
			continue;
		    }
		    s = p;
		    q = dp->d_name;
		    while ((*s++ = *q++) != '\0') {
			;		/* empty loop */
		    }
		    (*attr.func)();
		}
	    }
	}
    }
    else {
	while ((dp = readdir(dirp)) != (DIRTYPE *) NULL) {
	    if (isdot(dp->d_name) || isdotdot(dp->d_name)) {
		continue;
	    }
	    s = p;
	    q = dp->d_name;
	    while ((*s++ = *q++) != '\0') {
		;		/* empty loop */
	    }
	    (*attr.func)();
	}
    }
    
    (void) closedir(dirp);
    attr.depth--;
    *p = '\0';
    
    return;
}
#else

/*
 * fwt1:
 *	This function does the same thing as fwt1() above, but is
 *	meant for systems without a directory library, that does
 *	directory reading "by hand".
 *
 *    Below is the "no directory library" version of fwt1()
 *
 */

static void fwt1(int depth,
		 int dashr,
		 char *p,
		 int stat_all,
		 int nlink)
{
    FILE *		dirp;
    struct direct	dp;
    char *		q;
    char *		s;
    
    if (depth == 0) {
	if (dashr) {
	    warning("%s: file tree depth exceeded", attr.fname);
	}
	return;
    }
    attr.depth++;
    
    if ((dirp = fopen(attr.fname, "r")) == (FILE *) NULL) {
	warning("%s: %m", attr.fname);
	return;
    }
    
    nlink -= 2;

    if (stat_all) {
	if (attr.stat_all) {
	    while (fread(&dp, sizeof(struct direct), 1, dirp) == 1) {
		if (isdot(dp.d_name) || isdotdot(dp.d_name)) {
		    continue;
		}
		s = p;
		q = dp.d_name;
		while ((*s++ = *q++) != '\0') {
		    ;		/* empty loop */
		}
		s--;

		attr.prune = FALSE;
		if (!handle_file()) {
		    continue;
		}
		if (isproper(attr.buf->st_mode)) {
		    *s++ = '/';
		    *s = '\0';
		    fwt1(depth - 1, dashr, s,
			 attr.stat_all | hassubdir(attr.buf),
			 attr.buf->st_nlink);
		}
	    }
	}
	else {
	    while (fread(&dp, sizeof(struct direct), 1, dirp) == 1) {
		if (isdot(dp.d_name) || isdotdot(dp.d_name)) {
		    continue;
		}
		s = p;
		q = dp.d_name;
		while ((*s++ = *q++) != '\0') {
		    ;		/* empty loop */
		}
		s--;
		
		attr.prune = FALSE;
		if (!handle_file()) {
		    continue;
		}
		if (isproper(attr.buf->st_mode)) {
		    *s++ = '/';
		    *s = '\0';
		    fwt1(depth - 1, dashr, s,
			 attr.stat_all | hassubdir(attr.buf),
			 attr.buf->st_nlink);
		    nlink--;
		    if (nlink == 0) {
			break;
		    }
		}
	    }
	    if (dp != 1) {
		while (fread(&dp, sizeof(struct direct), 1, dirp) == 1) {
		    if (isdot(dp.d_name) || isdotdot(dp.d_name)) {
			continue;
		    }
		    s = p;
		    q = dp.d_name;
		    while ((*s++ = *q++) != '\0') {
			;		/* empty loop */
		    }
		    (*attr.func)();
		}
	    }
	}
    }
    else {
	while (fread(&dp, sizeof(struct direct), 1, dirp) == 1) {
	    if (isdot(dp.d_name) || isdotdot(dp.d_name)) {
		continue;
	    }
	    s = p;
	    q = dp.d_name;
	    while ((*s++ = *q++) != '\0') {
		;		/* empty loop */
	    }
	    (*attr.func)();
	}
    }
    
    (void) fclose(dirp);
    attr.depth--;
    *p = '\0';

    return;
}

#endif

/*
 * ftrw:
 *	Entry point to do the search, ftrw is a front end
 *	to the recursive fwt1.
 *	ftrw() initializes some global variables and
 *	builds the initial filename string which is passed to
 *	fwt1().
 */

static void _ftrw(char *file,
	  void (*fn)(),
	  int depth,
	  int dashr,
	  int samefsPC)
{
    char *name_end;

#if defined(DEBUG)
    if (Trace) {
	(void) fprintf(stderr, "ftrw(\"%s\")\n", file);
    }
#endif

    attr.prune = FALSE;
    attr.depth = 0;
    attr.func = fn;
    name_end = attr.fname;

    if (*file == '\0') {
	*name_end = '\0';
	if (stat(".", attr.buf) == 0) {
	    if (samefsPC != -1) {
		StackProgram[samefsPC].value.dev = attr.buf->st_dev;
	    }
	    fwt1(depth, dashr, name_end,
		 attr.stat_all | hassubdir(attr.buf), attr.buf->st_nlink);
	}
	else {
	    warning(".: %m");
	    attr.num_errors++;
	}
    }
    else {
	while ((*name_end++ = *file++) != '\0') {
	    ;		/* empty loop */
	}
	name_end--;
	
	if (!handle_file()) {
	    return;
	}
	
	if (*(name_end - 1) != '/') {
	    *name_end++ = '/';
	    *name_end = '\0';
	}
	if (isproper(attr.buf->st_mode)) {
	    if (samefsPC != -1) {
		StackProgram[samefsPC].value.dev = attr.buf->st_dev;
	    }
	    fwt1(depth, dashr, name_end,
		 attr.stat_all | hassubdir(attr.buf), attr.buf->st_nlink);
	}
    }
    
    return;
}

/*
 * ftrw()
 *	Wrapper around _ftrw(), above
 *
 * Allocate dynamic name/stat storage, free after processing
 */
void ftrw(char *file,
	  void (*fn)(),
	  int depth,
	  int dashr,
	  int samefsPC)
{
    char *filebuf;
    struct stat	*statbuf;

    attr.fname = filebuf = malloc(MAXPATHLEN + 1);
    attr.buf = statbuf = malloc(sizeof(struct stat));
    _ftrw(file, fn, depth, dashr, samefsPC);
    free(statbuf);
    free(filebuf);
    attr.fname = 0;
    attr.buf = 0;
}

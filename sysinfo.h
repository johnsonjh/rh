/*
 * sysinfo.h
 *
 * Written by:	Rick Ohnemus	(rick@sterling.com)
 *
 * rcsid: $Id: sysinfo.h,v 1.1 2008/12/27 00:56:03 vandys Exp $
 */

#if !defined(__SYSINFO_H__)
#define __SYSINFO_H__

typedef struct _group_info group_info;

struct _group_info {
   char *	name;
   gid_t	gid;
   group_info *	next;
};


typedef struct _user_info user_info;

struct _user_info {
   char *	name;
   uid_t	uid;
   user_info *	next;
};

extern group_info *	getgigid(gid_t gid);
extern group_info *	getginam(char *group_name);
extern user_info *	getuinam(char *user_name);
extern user_info *	getuiuid(uid_t uid);

#if defined(sun)

typedef struct _fs_info fs_info;

struct _fs_info {
    char *	fs_type;
    dev_t	fs_dev;
    char *	fs_dir;
    fs_info *	next;
};

extern fs_info *	getfidev(dev_t fs_dev);
extern int		mounted_fstype(char *type);

#endif	/* sun */

#endif	/* __SYSINFO_H__ */

/*
 * sysinfo.c
 *
 * Written by:	Rick Ohnemus	(rick@sterling.com)
 * 
 * This file contains functions for getting information about
 * users, groups, and file systems. Results are cached to speed
 * subsequent queries.
 *
 * Each cache is a simple linked list with new entries added
 * to the beginning of the list.
 *
 * Global Functions: getfidev, getgigid, getginam, getuinam,
 *		     getuiuid, mounted_fstype
 *
 * Static Functions: init_file_systems,
 */

#if !defined(lint)
static char rcsid[] = "$Id: sysinfo.c,v 1.1 2008/12/27 00:56:03 vandys Exp $";
#endif

#include <sys/types.h>
#include <string.h>
#include <grp.h>
#include <pwd.h>
#if defined(sun)
#include <mntent.h>
#endif

#include "alloc.h"
#include "sysinfo.h"

static group_info *	groups = (group_info *) NULL;
static user_info *	users = (user_info *) NULL;

#if defined(sun)

extern long strtol();

static fs_info *file_systems = (fs_info *) NULL;

/*
 * init_file_systems - initialize file systems cache with
 * information on all mounted file systems.
 */

static void init_file_systems(void)
{
    FILE *		mtab;
    char *		c_p;
    char *		dev_str;
    dev_t		device;
    fs_info *		p;
    fs_info *		prev;
    struct mntent *	info;
    
    if ((mtab = setmntent(MOUNTED, "r")) == (FILE *) NULL) {
	return;
    }
    while ((info = getmntent(mtab)) != (struct mntent *) NULL) {
	if ((dev_str = hasmntopt(info, "dev=")) == (char *) NULL) {
	    continue;
	}
	if (*dev_str == '0'
	    && (*(dev_str + 1) == 'x' || *(dev_str + 1) == 'X')) {
	    dev_str += 6;
	}
	else {
	    dev_str += 4;
	}
	device = (dev_t) strtol(dev_str, &c_p, 16);
	if (device == 0 || (c_p - dev_str) != 4) {
	    continue;
	}
	p = ALLOCATE(sizeof(fs_info));
	p->fs_type = SAVESTR(info->mnt_type);
	p->fs_dir = SAVESTR(info->mnt_dir);
	p->fs_dev = device;
	p->next = (fs_info *) NULL;
	if (file_systems == (fs_info *) NULL) {
	    file_systems = p;
	}
	else {
	    prev->next = p;
	}
	prev = p;
    }	
    (void) endmntent(mtab);
    return;
}

int mounted_fstype(type)
char *	type;
{
    fs_info *	p;
    
    if (file_systems == (fs_info *) NULL) {
	init_file_systems();
    }
    
    for (p = file_systems; p != (fs_info *) NULL; p = p->next) {
	if (strcmp(type, p->fs_type) == 0) {
	    return 1;
	}
    }
    
    return 0;
}

fs_info *getfidev(fs_dev)
dev_t	fs_dev;
{
    fs_info *	p;
    
    if (file_systems == (fs_info *) NULL) {
	init_file_systems();
    }
    
    for (p = file_systems; p != (fs_info *) NULL; p = p->next) {
	if (fs_dev == p->fs_dev) {
	    return p;
	}
    }
    
    return (fs_info *) NULL;
}
#endif

/*
 * getginam - get group information by group name
 */

group_info *getginam(char *group_name)
{
    struct group *	info;
    group_info *	p = groups;
    
    /* first check the group cache for the group name */
    
    while (p != (group_info *) NULL) {
	if (strcmp(group_name, p->name) == 0) {
	    return p;
	}
	p = p->next;
    }
    
    /*
     * The desired group name isn't in the group cache so, get group
     * information from the system and add it to the cache if
     * successful.
     */
    
    if ((info = getgrnam(group_name)) == (struct group *) NULL) {
	return (group_info *) NULL;
    }
    p = ALLOCATE(sizeof(group_info));
    p->name = SAVESTR(info->gr_name);
    p->gid = info->gr_gid;
    p->next = groups;
    groups = p;
    return p;
}

/*
 * getgigid - get group information by group id
 */

group_info *getgigid(gid_t gid)
{
    struct group *	info;
    group_info *		p = groups;
    
    /* first check the group cache for the group id */
    
    while (p != (group_info *) NULL) {
	if (gid == p->gid) {
	    return p;
	}
	p = p->next;
    }
    
    /*
     * The desired group id isn't in the group cache so, get group
     * information from the system and add it to the cache if
     * successful.
     */
    
    if ((info = getgrgid(gid)) == (struct group *) NULL) {
	return (group_info *) NULL;
    }
    p = ALLOCATE(sizeof(group_info));
    p->name = SAVESTR(info->gr_name);
    p->gid = info->gr_gid;
    p->next = groups;
    groups = p;
    return p;
}

/*
 * getuinam - get user information by user name
 */

user_info *getuinam(char *user_name)
{
    struct passwd *	info;
    user_info *		p = users;
    
    /* first check the user cache for the user name */
    
    while (p != (user_info *) NULL) {
	if (strcmp(user_name, p->name) == 0) {
	    return p;
	}
	p = p->next;
    }
    
    /*
     * The desired user name isn't in the user cache so, get user
     * information from the system and add it to the cache if
     * successful.
     */
    
    if ((info = getpwnam(user_name)) == (struct passwd *) NULL) {
	return (user_info *) NULL;
    }
    p = ALLOCATE(sizeof(user_info));
    p->name = SAVESTR(info->pw_name);
    p->uid = info->pw_uid;
    p->next = users;
    users = p;
    return p;
}

/*
 * getuiuid - get user information by user id
 */

user_info *getuiuid(uid_t uid)
{
    struct passwd *	info;
    user_info *		p = users;
    
    /* first check the user cache for the user id */
    
    while (p != (user_info *) NULL) {
	if (uid == p->uid) {
	    return p;
	}
	p = p->next;
    }
    
    /*
     * The desired user id isn't in the user cache so, get user
     * information from the system and add it to the cache if
     * successful.
     */
    
    if ((info = getpwuid(uid)) == (struct passwd *) NULL) {
	return (user_info *) NULL;
    }
    p = ALLOCATE(sizeof(user_info));
    p->name = SAVESTR(info->pw_name);
    p->uid = info->pw_uid;
    p->next = users;
    users = p;
    return p;
}

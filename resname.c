/*
 * resname.c
 *
 * Written by:	Rick Ohnemus	(rick@sterling.com)
 *
 * This file contains a function that finds a file given a colon
 * separated list of directories to search and a file name.
 * The resolved name is returned in allocated memory.
 *
 * Global Functions: resolve_name
 */

#if !defined(lint)
static char rcsid[] = "$Id: resname.c,v 1.1 2008/12/27 00:56:03 vandys Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "alloc.h"

#if !defined(MAX_PATH_LEN)
#  define MAX_PATH_LEN	1024
#endif

char *resolve_name(const char *ro_paths, const char *name)
{
    int		fd;
    char	file_name[MAX_PATH_LEN + 1];
    int		name_len;
    char *	p;
    char *	paths;
    char *	pathbufp;
    
    if (ro_paths == (char *) NULL || strchr(name, '/') != NULL) {
	return SAVESTR(name);
    }

    pathbufp = paths = SAVESTR(ro_paths);
    
    name_len = strlen(name);
    
    while (*pathbufp != '\0') {
	if ((p = strchr(pathbufp, ':')) != (char *) NULL) {
	    *p = '\0';
	}
	if ((strlen(pathbufp) + name_len + 1) <= MAX_PATH_LEN) {
	    (void) sprintf(file_name, "%s/%s",
			   (*pathbufp == '\0') ? "." : pathbufp, name);
	    if ((fd = open(file_name, O_RDONLY)) >= 0) {
		(void) close(fd);
		free(paths);
		return SAVESTR(file_name);
	    }
	}
	if (p == (char *) NULL) {
	    break;
	}
	pathbufp = p + 1;
    }
    
    free(paths);
    return SAVESTR(name);
}

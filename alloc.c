/*
 * alloc.c
 *
 * Written by: Rick Ohnemus	(rick@sterling.com)
 *
 * This file contains functions related to dynamic memory allocation.
 *
 * Global Functions: allocate, reallocate, savestr
 */

#if !defined(lint)
static char rcsid[] = "$Id: alloc.c,v 1.1 1994/01/12 19:33:22 rick Exp $";
#endif

#include <stdlib.h>
#include <string.h>

#include "errmsg.h"

/*
 * allocate:
 *	Allocate 'n' bytes of memory. If the allocation fails a
 *	message is displayed containing the name of the source
 *	file and line number in the file that allocate() was
 *	called from.
 */

void *allocate(size_t n, const char *file, const int line)
{
    void *p = malloc(n);
    if (p == NULL) {
	error("%s : %d : no space : %m", file, line);
    }
    return p;
}

/*
 * reallocate:
 *	Reallocate 'n' bytes of memory. If the address to
 *	reallocate is null then just call allocate() to
 *	allocate a new block of memory.
 *	If the allocation fails a message is displayed
 *	containing the name of the source file and line
 *	number in the file that reallocate() was called from.
 */

void *reallocate(void *a, size_t n, const char *file, const int line)
{
    void *p;

    if (a == NULL) {
	return allocate(n, file, line);
    }

    p = realloc(a, n);
    if (p == NULL) {
	error("%s : %d : no space : %m", file, line);
    }

    return p;
}

/*
 * savestr:
 *	Allocate space for a string then copy the string
 *	into the newly allocated memory.
 */

char *savestr(const char *str, const char *file, const int line)
{
    return strcpy(allocate(strlen(str) + 1, file, line), str);
}

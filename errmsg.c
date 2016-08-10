/*
 * errmsg.c
 *
 * Written by:	Rick Ohnemus	(rick@sterling.com)
 *
 * This file contains functions to write messages to stderr with
 * program information and expanded errno messages.
 *
 * Global Functions: error, warning
 *
 * Static Functions: percent_m, errmsg
 */

#if !defined(lint)
static char rcsid[] = "$Id: errmsg.c,v 1.2 2010/04/22 03:02:46 vandys Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

const char *			Program_name;
#include <errno.h>

/*
 * percent_m:
 *	Return pointer to '%m' in a string or (char *) NULL if
 *	it is not found. '%' escaping is checked. This means
 *	that " %%m " does not contain '%m' since the '%' is
 *	escaped.
 */

static const char *percent_m(const char *str)
{
    while (*str != '\0') {
	if (*str == '%') {
	    if (*(str + 1) == 'm') {
		return str;
	    }
	    else if (*(str + 1) != '\0') {
		str++;
	    }
	}
	str++;
    }

    return NULL;
}

/*
 * errmsg:
 *	Write a message to stderr with the program name at the
 *	beginning of the message. If '%m' is found in the
 *	message then replace it with text describing the value
 *	in errno. Finally output a line feed at the end of the
 *	message if it does not end with a line feed.
 */

static void errmsg(const char *format, va_list args)
{
    const char *	m_ptr;
    char *		nfp;
    const char *	p;
    char		new_format[BUFSIZ * 2];
    int			save_errno = errno;


    (void) fprintf(stderr, "%s: ", Program_name);
    
    if ((m_ptr = percent_m(format)) != NULL) {
	(void) strncpy(new_format, format, m_ptr - format);
	nfp = new_format + (m_ptr - format);
	p = strerror(save_errno);
	if (p) {
	    while (*p) {
		*nfp++ = *p++;
	    }
	} else {
	    (void) sprintf(nfp, "Unknown error : %d", save_errno);
	    nfp += strlen(nfp);
	}
	(void) strcpy(nfp, m_ptr + 2);
	format = new_format;
    }
    
    (void) vfprintf(stderr, format, args);
    
    for (p = format; *p != '\0'; p++) {
	;		/* empty loop */
    }
    
    if (*(p - 1) != '\n') {
	(void) putc('\n', stderr);
    }
    
    (void) fflush(stderr);

    return;
}

/*
 * error:
 *	Calls errmsg() and exits.
 */

void error(const char *format, ...)
{
    va_list args;
    
    va_start(args, format);
    (void) fflush(stdout);
    errmsg(format, args);
    va_end(args);
    exit(1);
    /*NOTREACHED*/
}

/*
 * warning:
 *	Calls errmsg() and returns.
 */

void warning(const char *format, ...)
{
    va_list args;
    
    va_start(args, format);
    (void) fflush(stdout);
    errmsg(format, args);
    va_end(args);
    return;
}

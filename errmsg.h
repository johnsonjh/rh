/*
 * errmsg.h
 *
 * WRITTEN BY:	Rick Ohnemus	(rick@sterling.com)
 *
 * rcsid: $Id: errmsg.h,v 1.1 1994/01/12 19:35:00 rick Exp $
 */

#if !defined(__ERRMSG_H__)
#define __ERRMSG_H__

extern const char *Program_name;

extern void error(const char *format, ...);
extern void warning(const char *format, ...);

#endif	/* __ERRMSG_H__ */

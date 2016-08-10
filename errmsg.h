/*
 * errmsg.h
 *
 * WRITTEN BY:	Rick Ohnemus	(rick@sterling.com)
 *
 * rcsid: $Id: errmsg.h,v 1.1 2008/12/27 00:56:03 vandys Exp $
 */

#if !defined(__ERRMSG_H__)
#define __ERRMSG_H__

extern const char *Program_name;

extern void error(const char *format, ...);
extern void warning(const char *format, ...);

#endif	/* __ERRMSG_H__ */

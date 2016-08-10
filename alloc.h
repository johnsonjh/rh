/*
 * alloc.h
 *
 * Written by:	Rick Ohnemus	(rick@sterling.com)
 *
 * rcsid: $Id: alloc.h,v 1.1 2008/12/27 00:56:03 vandys Exp $
 */

#if !defined(__ALLOC_H__)
#define __ALLOC_H__

#define ALLOCATE(n)	 allocate((n), __FILE__, __LINE__)
#define REALLOCATE(a, n) reallocate((a), (n), __FILE__, __LINE__)
#define SAVESTR(str)	 savestr((str), __FILE__, __LINE__)

extern void *allocate(size_t n, const char *file, const int line);
extern void *reallocate(void *a, size_t n, const char *file, const int line);
extern char *savestr(const char *str, const char *file, const int line);

#endif	/* __ALLOC_H__ */

/*
 * getdate.h
 *
 * Written by:	Rick Ohnemus	(rick@sterling.com)
 *
 * This file contains the structure returned by getdate().
 *
 * rcsid: $Id: getdate.h,v 1.1 1994/01/12 19:35:41 rick Exp $
 */

#if !defined(__GETDATE_H__)
#define __GETDATE_H__

typedef struct {
    time_t		seconds;

    time_t		date;
    time_t		time;
    time_t		zone;

    unsigned int	dateflag  : 1;
    unsigned int	timeflag  : 1;
    unsigned int	zoneflag  : 1;

    unsigned int	hourflag  : 1;
    unsigned int	minflag   : 1;
    unsigned int	secflag   : 1;
    unsigned int	monthflag : 1;
    unsigned int	yearflag  : 1;
    unsigned int	dayflag   : 1;

} date_info;

extern date_info getdate();

#endif	/* __GETDATE_H__ */

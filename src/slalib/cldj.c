/* cldj.f -- translated by f2c (version 20160102).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "f2c.h"

/* Subroutine */ int sla_cldj__(integer *iy, integer *im, integer *id, 
	doublereal *djm, integer *j)
{
    /* Initialized data */

    static integer mtab[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

/* + */
/*     - - - - - */
/*      C L D J */
/*     - - - - - */

/*  Gregorian Calendar to Modified Julian Date */

/*  Given: */
/*     IY,IM,ID     int    year, month, day in Gregorian calendar */

/*  Returned: */
/*     DJM          dp     modified Julian Date (JD-2400000.5) for 0 hrs */
/*     J            int    status: */
/*                           0 = OK */
/*                           1 = bad year   (MJD not computed) */
/*                           2 = bad month  (MJD not computed) */
/*                           3 = bad day    (MJD computed) */

/*  The year must be -4699 (i.e. 4700BC) or later. */

/*  The algorithm is adapted from Hatcher 1984 (QJRAS 25, 53-55). */

/*  Last revision:   27 July 2004 */

/*  Copyright P.T.Wallace.  All rights reserved. */

/*  License: */
/*    This program is free software; you can redistribute it and/or modify */
/*    it under the terms of the GNU General Public License as published by */
/*    the Free Software Foundation; either version 2 of the License, or */
/*    (at your option) any later version. */

/*    This program is distributed in the hope that it will be useful, */
/*    but WITHOUT ANY WARRANTY; without even the implied warranty of */
/*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/*    GNU General Public License for more details. */

/*    You should have received a copy of the GNU General Public License */
/*    along with this program (see SLA_CONDITIONS); if not, write to the */
/*    Free Software Foundation, Inc., 59 Temple Place, Suite 330, */
/*    Boston, MA  02111-1307  USA */

/* - */
/*  Month lengths in days */
//printf("in fortran routine: year %d month %d day %d", iy, im, id);
/*  Preset status. */
    *j = 0;
/*  Validate year. */
    if (*iy < -4699) {
	*j = 1;
    } else {
/*     Validate month. */
	if (*im >= 1 && *im <= 12) {
/*        Allow for leap year. */
	    if (*iy % 4 == 0) {
		mtab[1] = 29;
	    } else {
		mtab[1] = 28;
	    }
	    if (*iy % 100 == 0 && *iy % 400 != 0) {
		mtab[1] = 28;
	    }
/*        Validate day. */
	    if (*id < 1 || *id > mtab[*im - 1]) {
		*j = 3;
	    }
/*        Modified Julian Date. */
	    *djm = (doublereal) ((*iy - (12 - *im) / 10 + 4712) * 1461 / 4 + (
		    (*im + 9) % 12 * 306 + 5) / 10 - (*iy - (12 - *im) / 10 + 
		    4900) / 100 * 3 / 4 + *id - 2399904);
/*        Bad month. */
	} else {
	    *j = 2;
	}
    }
    return 0;
} /* sla_cldj__ */


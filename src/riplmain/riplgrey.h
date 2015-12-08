/*
 *		RIPL---Richard's Image-Processing Library.
 *		Written by Richard Cook.
 *
 *		riplgrey.h
 *		Header file declaring the greymap data type and greymap-handling
 *		functions---functions to instantiate, resize and destroy greymap
 *		objects among other things.
 *
 *		Version 1.1, last update: 20 January 1998.
 *
 *		History:
 *			20/1/98:		version 1.1.
 *			24/12/97:	modified included-file list.
 *			27/11/97:	first serious version.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#ifndef _RIPLGREY_H_INCLUDED
#define _RIPLGREY_H_INCLUDED

#include "ripldefs.h"
#include "riplmisc.h"
#include "Image.h"

using RiplGreyMap = ripl::Image<ripl::grey_t>;

typedef struct tagriplGreyMap {
    unsigned cols, rows;
    unsigned long size;
    riplGrey *data;
} riplGreyMap;

#endif

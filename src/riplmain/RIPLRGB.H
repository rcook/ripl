/*
 *		RIPL---Richard's Image-Processing Library.
 *		Written by Richard Cook.
 *
 *		riplrgb.h
 *		Header file declaring the RGBMap data type.
 *
 *		Version 1.1, last update: 20 January 1998.
 *
 *		History:
 *			20/1/98:		version 1.1.
 *			24/12/97:	first version.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#ifndef _RIPLRGB_H_INCLUDED
#define _RIPLRGB_H_INCLUDED

#include "ripldefs.h"

typedef enum tagriplRGBComponent {
	rgbcRed,
	rgbcGreen,
	rgbcBlue,
	rgbcMono
} riplRGBComponent;

typedef struct tagriplRGBMap {
	unsigned cols, rows;
	unsigned long size;
	riplGrey *red,
		*green,
		*blue,
		*mono;
} riplRGBMap;

#endif

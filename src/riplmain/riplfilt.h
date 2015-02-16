/*
 *		RIPL---Richard's Image-Processing Library.
 *		Written by Richard Cook.
 *
 *		riplfilt.h
 *		Header file for graphic import/export filters and support functions.
 *
 *		Version 1.1, last update: 20 January 1998.
 *
 *		History:
 *			20/1/98:		version 1.1.
 *			27/11/97:	first version.
 *		Copyright © 1997/8, Richard A. Cook.
 */
#ifndef _RIPLFILT_H_INCLUDED
#define _RIPLFILT_H_INCLUDED

#include "ripldefs.h"
#include "riplgrey.h"

riplGreyMap *riplLoadImage(const char *pfileName);
riplBool riplSaveImage(const char *pfileName,
	riplGraphicFormat graphicFormat,
	const riplGreyMap *pgreyMap);
riplGraphicFormat riplReadGraphicFormat(const char *pfileName);

#endif

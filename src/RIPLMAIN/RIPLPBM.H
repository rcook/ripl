/*
 *		RIPL---Richard's Image-Processing Library.
 *		Written by Richard Cook.
 *
 *		riplpbm.h
 *		Header file for Portable Bitmap (pbm/PGM/PPM) import filter functions.
 *
 *		Version 1.1, last update: 20 January 1998.
 *
 *		History:
 *			20/1/98:			version 1.1.
 *			27/11/97:		first version.
 *		Copyright © 1997/8, Richard A. Cook.
 */
#ifndef _RIPLPBM_H_INCLUDED
#define _RIPLPBM_H_INCLUDED

#include "ripldefs.h"
#include "riplgrey.h"

riplGreyMap *riplPBMLoadFile(const char *pfileName,
	riplGraphicFormat graphicFormat);
riplBool riplPBMSaveFile(const char *pfileName,
	riplGraphicFormat graphicFormat,
	const riplGreyMap *pgreyMap);

#endif

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

#pragma once

#include "ripldefs.h"
#include "riplgrey.h"

riplGreyMap riplPBMLoadFile(
    const char* fileName,
    riplGraphicFormat graphicFormat);

void riplPBMSaveFile(
    const char* fileName,
    riplGraphicFormat graphicFormat,
    const riplGreyMap& greyMap);

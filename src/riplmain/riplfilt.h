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

#pragma once

#include "ripldefs.h"
#include "riplgrey.h"

riplGreyMap riplLoadImage(const char* fileName);

void riplSaveImage(
    const char* fileName,
    riplGraphicFormat graphicFormat,
    const riplGreyMap& greyMap);

riplGraphicFormat riplReadGraphicFormat(const char* fileName);

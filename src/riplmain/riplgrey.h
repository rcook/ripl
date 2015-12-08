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

#pragma once

#include "riplmisc.h"
#include "Image.h"

using riplGreyMap = ripl::Image<ripl::grey_t>;

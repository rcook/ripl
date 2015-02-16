/*
 *		RIPL---Richard's Image-Processing Library.
 *		Written by Richard Cook.
 *
 *		ripl.h
 *		Header file includes all Ripl headers.
 *
 *		Version 1.1, last update: 20 January 1998.
 *
 *		History:
 *			20/1/98:		version 1.1.
 *			20/1/98:		modified included files list.
 *			27/11/97:	first implemented.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#ifndef _RIPL_H_INCLUDED
#define _RIPL_H_INCLUDED

#include "ripldefs.h"

#include "riplallc.h"
#include "ripldbug.h"
#include "riplfilt.h"
#include "riplgrey.h"
#include "riplimg.h"
#include "riplmisc.h"
#include "riplmsg.h"
#include "riplpars.h"
#include "riplpbm.h"
#include "riplrgb.h"

int riplMain1(unsigned argc, char **argv);
int riplMain2(unsigned argc, char **argv,
	riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap);

#endif

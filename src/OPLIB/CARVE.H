/*
 *		carve.h
 *		Header file for carving operator.
 *
 *		Version 1.1, last update: 24 January 1998.
 *
 *		History:
 *			24/1/98:		introduced RIPL_PARAMERROR.
 *			16/12/97:	updated comments.
 *			15/12/97:	first implemented.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#ifndef _CARVE_H_INCLUDED
#define _CARVE_H_INCLUDED

#include "ripl.h"

/* Command-line version. */
int carveExecute(unsigned argc,
	const char **argv,
	riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap);
const char *carveHelp(void);
/* Internal entrypoint. */
riplBool carveApplyOperator(riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap,
	unsigned win_width,
	unsigned win_height,
	unsigned min_width,
	unsigned min_height,
	unsigned nclasses,
	riplGrey conf);

#endif


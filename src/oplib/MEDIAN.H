/*
 *		median.h
 *		Header file for median filtering.
 *
 *		Version 1.1, last update: 24 January 1998.
 *
 *		History:
 *			24/1/98:		introduced RIPL_PARAMERROR.
 *			16/12/97:	updated comments.
 *			27/11/97:	first implemented.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#ifndef _MEDIAN_H_INCLUDED
#define _MEDIAN_H_INCLUDED

#include "ripl.h"

/* Command-line version. */
int medianExecute(unsigned argc,
	const char **argv,
	riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap);
const char *medianHelp(void);
/* Internal entrypoint. */
riplBool medianApplyOperator(riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap,
	unsigned window_size);

#endif


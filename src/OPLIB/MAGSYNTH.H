/*
 *		magsynth.h
 *		Header file for magnitude-only synthesis operator.
 *
 *		Version 1.1, last update: 16 February 1998.
 *
 *		History:
 *			16/2/98:		first implemented.
 *
 *		Copyright © 1998, Richard A. Cook.
 */
#ifndef _MAGSYNTH_H_INCLUDED
#define _MAGSYNTH_H_INCLUDED

#include "ripl.h"

/* Command-line version. */
int magsynthExecute(unsigned argc,
	const char **argv,
	riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap);
const char *magsynthHelp(void);
/* Internal entrypoint. */
riplBool magsynthApplyOperator(riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap);

#endif


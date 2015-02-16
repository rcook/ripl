/*
 *		gauss.h
 *		Header file for Gaussian smoothing filter.
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
#ifndef _GAUSS_H_INCLUDED
#define _GAUSS_H_INCLUDED

#include "ripl.h"

/* Command-line version. */
int gaussExecute(unsigned argc,
    const char **argv,
    riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap);
const char *gaussHelp(void);
/* Internal entrypoint. */
riplBool gaussApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    double variance);

#endif


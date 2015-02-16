/*
 *		wvthrsh.c
 *		Source file for wavelet-thresholding operator.
 *
 *		Version 1.1, last update: 2 February 1998.
 *
 *		History:
 *			2/2/98:		first implemented.
 *
 *		Copyright © 1998, Richard A. Cook.
 */
#ifndef _WVTHRSH_H_INCLUDED
#define _WVTHRSH_H_INCLUDED

#include "ripl.h"
#include "riplwave.h"

/* Command-line version. */
int wavethreshExecute(unsigned argc,
    const char **argv,
    riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap);
const char *wavethreshHelp(void);
/* Internal entrypoint. */
riplBool wavethreshApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    riplwtFilterType filter_type,
    float threshold);

#endif


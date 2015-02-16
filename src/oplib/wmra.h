/*
 *		wmra.c
 *		Source file for wavelet-based multiresolution analysis operator.
 *
 *		Version 1.1, last update: 10 February 1998.
 *
 *		History:
 *			10/2/98:		first implemented.
 *
 *		Copyright © 1998, Richard A. Cook.
 */
#ifndef _WMRA_H_INCLUDED
#define _WMRA_H_INCLUDED

#include "ripl.h"
#include "riplwave.h"

/* Command-line version. */
int wmraExecute(unsigned argc,
    const char **argv,
    riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap);
const char *wmraHelp(void);
/* Internal entrypoint. */
riplBool wmraApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    riplwtFilterType filter_type,
    float threshold);

#endif


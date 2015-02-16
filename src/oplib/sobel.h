/*
 *		sobel.h
 *		Header file for Sobel edge detection.
 *
 *		Version 1.1, last update: 24 January 1998.
 *
 *		History:
 *			24/1/98:		introduced RIPL_PARAMERROR.
 *			16/12/97:	updated comments.
 *			27/11/97:	first implemented.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 *
 *		Based on the algorithm in 'Practical Image Processing in C' by
 *		Craig A. Lindley. (This is an enhanced and more general version
 *		of the routine---unfortunately it's still anisotropic).
 */
#ifndef _SOBEL_H_INCLUDED
#define _SOBEL_H_INCLUDED

#include "ripl.h"

/* Command-line version. */
int sobelExecute(unsigned argc,
    const char **argv,
    riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap);
const char *sobelHelp(void);
/* Internal entrypoint. */
riplBool sobelApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    unsigned neighbourhood,
    riplGrey threshold,
    riplBool overlay);

#endif


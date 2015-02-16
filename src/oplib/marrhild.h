/*
 *		marrhild.h
 *		Header file for Laplacian-of-Gaussian (Marr-Hildreth) edge detection.
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
#ifndef _MARRHILD_H_INCLUDED
#define _MARRHILD_H_INCLUDED

#include "ripl.h"

/* Command-line version. */
int marrhildExecute(unsigned argc,
    const char **argv,
    riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap);
const char *marrhildHelp(void);
/* Internal entrypoint. */
riplBool marrhildApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    double variance);

#endif


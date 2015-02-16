/*
 *		siahe.h
 *		Header file for adaptive histogram equalization operator.
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
#ifndef _SIAHE_H_INCLUDED
#define _SIAHE_H_INCLUDED

#include "ripl.h"
#include "misc.h"

/* Command-line version. */
int siaheExecute(unsigned argc,
    const char **argv,
    riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap);
const char *siaheHelp(void);
/* Internal entrypoint. */
riplBool siaheApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    miscWindowFunction win_func,
    unsigned window_size);

#endif


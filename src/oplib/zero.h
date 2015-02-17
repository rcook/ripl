/*
 *		zero.h
 *		Header file for zeroing operator.
 *
 *		Version 1.1, last update: 24 January 1998.
 *
 *		History:
 *			24/1/98:		introduced use of RIPL_PARAMERROR.
 *			16/12/97:	updated comments.
 *			27/11/97:	first implemented.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#ifndef _ZERO_H_INCLUDED
#define _ZERO_H_INCLUDED

#include "ripl.h"

/* Command-line version. */
int zeroExecute(unsigned argc,
    const char **argv,
    riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap);
const char *zeroHelp(void);
/* Internal entrypoint. */
bool zeroApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    riplGrey threshold);

#endif


/*
 *		globalhe.h
 *		Header file for global histogram equalization operator.
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
#ifndef _GLOBALHE_H_INCLUDED
#define _GLOBALHE_H_INCLUDED

#include "ripl.h"

/* Command-line version. */
int globalHEExecute(unsigned argc,
    const char **argv,
    riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap);
const char *globalHEHelp(void);
/* Internal entrypoint. */
riplBool globalHEApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap);

#endif


/*
 *		phsynth.h
 *		Header file for phase-only synthesis operator.
 *
 *		Version 1.1, last update: 16 February 1998.
 *
 *		History:
 *			16/2/98:		first implemented.
 *
 *		Copyright © 1998, Richard A. Cook.
 */
#ifndef _PHSYNTH_H_INCLUDED
#define _PHSYNTH_H_INCLUDED

#include "ripl.h"

/* Command-line version. */
int phasesynthExecute(unsigned argc,
    const char **argv,
    riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap);
const char *phasesynthHelp(void);
/* Internal entrypoint. */
riplBool phasesynthApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap);

#endif


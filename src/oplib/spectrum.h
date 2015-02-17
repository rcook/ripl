/*
 *		spectrum.h
 *		Header file for Spectrum operator.
 *
 *		Version 1.1, last update: 24 January 1998.
 *
 *		History:
 *			24/1/98:		slight modifications.
 *			23/1/98:		operator renamed 'spectrum'.
 *			21/1/98:		actually made it do something.
 *			16/12/97:	updated comments.
 *			27/11/97:	first implemented.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#ifndef _SPECTRUM_H_INCLUDED
#define _SPECTRUM_H_INCLUDED

#include "ripl.h"

/* Command-line version. */
int spectrumExecute(unsigned argc,
    const char **argv,
    riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap);
const char *spectrumHelp(void);
/* Internal entrypoint. */
bool spectrumApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    bool subtract_mean);

#endif


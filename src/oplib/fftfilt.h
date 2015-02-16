/*
 *		fftfilt.h
 *		Header file for FFT-based filtering operator.
 *
 *		Version 1.1, last update: 24 January 1998.
 *
 *		History:
 *			24/1/98:		slight modifications.
 *			19/1/98:		first implemented.
 *
 *		Copyright © 1998, Richard A. Cook.
 */
#ifndef _FFTFILT_H_INCLUDED
#define _FFTFILT_H_INCLUDED

#include "ripl.h"

/* Command-line version. */
int fftfiltExecute(unsigned argc,
	const char **argv,
	riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap);
const char *fftfiltHelp(void);
/* Internal entrypoint. */
riplBool fftfiltApplyOperator(riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap,
	float *pfreqResp,
	riplBool is_complex);

#endif


/*
 *		ahe.h
 *		Header file for adaptive histogram equalization based on JASHE.
 *
 *		Version 1.1, last update: 3 May 1998.
 *
 *		History:
 *			3/5/98:		minor bug fix (hamming option not correctly parsed)
 *			24/1/98:		introduced RIPL_PARAMERROR.
 *			23/1/98:		minor modification: M_PI --> RIPL_PI etc.
 *			24/12/97:	updated on-line info.
 *			16/12/97:	updated comments.
 *			15/12/97:	first implemented.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#ifndef _AHE_H_INCLUDED
#define _AHE_H_INCLUDED

#include "ripl.h"

typedef enum tagaheHEType{
	htStandard,
	htFraction
} aheHEType;

typedef enum tagaheAddbackType {
	atNone,
	atZeroInput,
	atLocalMean
} aheAddbackType;

/* Configuration information. */
typedef struct tagaheConfig {
	unsigned num_terms;
	unsigned win_w, win_h;
	riplBool hamming_window;
	riplBool prescale_mode;
	float prescale_val;
	aheHEType he_type;
	float he_fraction;
	aheAddbackType addback_mode;
	float addback_fraction;
	riplBool addback_cum;
	riplBool gauss_mode;
	float gauss_width;
	float inoffset;
} aheConfig;

/* Command-line version. */
int aheExecute(unsigned argc,
	const char **argv,
	riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap);
const char *aheHelp(void);
/* Internal entrypoint. */
riplBool aheApplyOperator(riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap,
	const aheConfig *pconfig);
void aheInitializeConfig(aheConfig *pconfig);

#endif

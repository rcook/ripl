/*
 *		mask.h
 *		Header file for masking operator.
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
#ifndef _MASK_H_INCLUDED
#define _MASK_H_INCLUDED

#include "ripl.h"

/* Command-line version. */
int maskExecute(unsigned argc,
	const char **argv,
	riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap);
const char *maskHelp(void);
/* Internal entrypoint. */
riplBool maskApplyOperator(riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap,
	riplGrey mask);

#endif


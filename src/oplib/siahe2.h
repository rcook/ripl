/*
 *		siahe2.h
 *		Header file for extended raised-cosine subimage AHE as described in
 *		an appendix of `Image-Processing Techniques with Application in Image
 *		Restoration' by Richard A. Cook.
 *
 *		Usage: ripl <in-file> <out-file> siahe2 <w-r> <w-c> <d-r> <d-c> <wp>
 *
 *		Version 1.1, last update: 9 May 1998.
 *
 *		History:
 *			9/5/98:		first implemented.
 *
 *		Copyright © 1998, Richard A. Cook.
 */
#ifndef _SIAHE2_H_INCLUDED
#define _SIAHE2_H_INCLUDED

#include "ripl.h"
#include "misc.h"

/* Command-line version. */
int siahe2Execute(unsigned argc,
	const char **argv,
	riplGreyMap *pinputgrey,
	riplGreyMap *poutputgrey);
const char *siahe2Help(void);
/* Internal entrypoint. */
riplBool siahe2ApplyOperator(riplGreyMap *pinputgrey,
	riplGreyMap *poutputgrey,
	unsigned w_r,
	unsigned w_c,
	unsigned d_r,
	unsigned d_c,
	miscWindowProfile wp);

#endif


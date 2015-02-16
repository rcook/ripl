/*
 *		plane.h
 *		Header file for bit-plane extraction operator.
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
#ifndef _PLANE_H_INCLUDED
#define _PLANE_H_INCLUDED

#include "ripl.h"

/* Command-line version. */
int planeExecute(unsigned argc,
    const char **argv,
    riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap);
const char *planeHelp(void);
/* Internal entrypoint. */
riplBool planeApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    riplGrey bitplane);

#endif


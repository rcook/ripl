/*
 *		add.h
 *		Header file for adding operator.
 *
 *		Version 1.1, last update: 24 January 1998.
 *
 *		History:
            24/1/98:		introduced RIPL_PARAMERROR.
 *			16/12/97:	updated comments.
 *			15/12/97:	first implemented.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#ifndef _ADD_H_INCLUDED
#define _ADD_H_INCLUDED

#include "ripl.h"

void addImages(
    riplGreyMap& output,
    const riplGreyMap& input0,
    const riplGreyMap& input1,
    float weight);

/* Command-line version. */
int addExecute(unsigned argc,
    const char **argv,
    riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap);
const char *addHelp(void);
/* Internal entrypoint. */
bool addApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    float weight,
    const char *pfileName);

#endif


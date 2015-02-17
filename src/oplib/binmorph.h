/*
 *		binmorph.h
 *		Header file for binary morphological operators.
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
#ifndef _BINMORPH_H_INCLUDED
#define _BINMORPH_H_INCLUDED

#include "ripl.h"
#include "misc.h"

/* Command-line version. */
int binmorphExecute(unsigned argc,
    const char **argv,
    riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap);
const char *binmorphHelp(void);
/* Internal entrypoint. */
bool binmorphApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    miscOperatorType operator_type,
    miscPredefinedStruct predefined_struct,
    unsigned struct_el_cols,
    unsigned struct_el_rows,
    bool *struct_el);

#endif

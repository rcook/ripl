/*
 *		glmorph.h
 *		Header file for grey-level morphological operators.
 *
 *		Version 1.1, last update: 24 January 1998.
 *
 *		History:
 *			24/1/98:		introduced RIPL_PARAMERROR.
 *			16/12/97:	updated comments.
 *			27/11/97:	first implemented.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 *
 *		Based loosely on the Matlab routines 'gerode.m' and 'gdilate.m'
 *		in P.Y.Yee's EIST Project Report (1993/4 I think). In this version
 *		I assume the foreground is light and background dark (opposite of
 *		Yee). This corresponds to the convention adopted by Paint Shop Pro.
 */
#ifndef _GLMORPH_H_INCLUDED
#define _GLMORPH_H_INCLUDED

#include "ripl.h"
#include "misc.h"

/* Command-line version. */
int glmorphExecute(unsigned argc,
	const char **argv,
	riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap);
const char *glmorphHelp(void);
/* Internal entrypoint. */
riplBool glmorphApplyOperator(riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap,
	miscOperatorType operator_type,
	miscPredefinedStruct predefined_struct,
	unsigned struct_el_cols,
	unsigned struct_el_rows,
	riplBool *struct_el);

#endif

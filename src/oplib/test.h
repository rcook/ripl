/*
 *		test.h
 *		Test RConsole header file.
 *
 *		Version 1.1, last update: 24 January 1998.
 *
 *		History:
 *			24/1/98:		introduced RIPL_PARAMERROR.
 *			16/12/97:	updated comments.
 *			6/12/97:		first implemented.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#ifndef _TEST_H_INCLUDED
#define _TEST_H_INCLUDED

#include "ripl.h"

/* Command-line version. */
int testExecute(unsigned argc,
	const char **argv,
	riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap);
const char *testHelp(void);
/* Internal entrypoint. */
riplBool testApplyOperator(riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap);

#endif


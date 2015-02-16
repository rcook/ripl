/*
 *		globalhe.c
 *		Source file for global histogram equalization operator.
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
#include "globalhe.h"
#include "misc.h"

/* Internal entrypoint. */
riplBool globalHEApplyOperator(riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap) {

	RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap)
	return miscHistogramEQ(pinputGreyMap->data, poutputGreyMap->data,
		pinputGreyMap->size);
}

/* Command-line version. */
#ifdef __BORLANDC__
#	pragma argsused
#endif
int globalHEExecute(unsigned argc, const char **argv,
	riplGreyMap *pinputGreyMap, riplGreyMap *poutputGreyMap) {

	if (!globalHEApplyOperator(pinputGreyMap, poutputGreyMap))
		return RIPL_EXECUTEERROR;
	return 0;
}

/* Display help screen. */
const char *globalHEHelp(void) {
	return "globalhe\n\n"
		"   Applies simplified global histogram equalization to the image.\n";
}


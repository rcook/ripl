/*
 *		thresh.c
 *		Source file for thresholding operator.
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
#include "thresh.h"

/* Internal entrypoint. */
riplBool thresholdApplyOperator(riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap,
	riplGrey threshold) {

	const riplGrey *inP=pinputGreyMap->data;
	riplGrey *outP=poutputGreyMap->data;
	unsigned i;

	RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap)
	
	for (i=0; i<pinputGreyMap->size; i++, inP++, outP++) {
		if (*inP<threshold) *outP=0;
		else *outP=RIPL_MAX_GREY;
	}
	return true;
}

/* Command-line version. */
int thresholdExecute(unsigned argc, const char **argv,
	riplGreyMap *pinputGreyMap, riplGreyMap *poutputGreyMap) {
	riplGrey threshold;
	if (argc<1) {
		riplMessage(itError, "Incorrect number of parameters!\n"
			"Usage: " RIPL_EXENAME " " RIPL_CMDLINE
			" threshold <level>\n");
		return RIPL_PARAMERROR;
	}
	if (!riplArgGet_riplGrey(argv[0], &threshold)) {
		riplMessage(itError, "threshold: <level> should be a byte value.\n");
		return RIPL_PARAMERROR;
	}
	if (!thresholdApplyOperator(pinputGreyMap, poutputGreyMap, threshold))
		return RIPL_EXECUTEERROR;
	return 1;
}

/* Display help screen. */
const char *thresholdHelp(void) {
	return " threshold <level>\n\n"
		"   Thresholds the image such that grey levels below <level> are\n"
		"   set to black (0) and grey levels equal to or above <level> are\n"
		"   set to white (255).\n";
}


/*
 *		zero.c
 *		Source file for zeroing operator.
 *
 *		Version 1.1, last update: 24 January 1998.
 *
 *		History:
 *			24/1/98:		introduced use of RIPL_PARAMERROR.
 *			16/12/97:	updated comments.
 *			27/11/97:	first implemented.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#include "zero.h"

/* Internal entrypoint. */
riplBool zeroApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    riplGrey threshold) {

    const riplGrey *inP=pinputGreyMap->data;
    riplGrey *outP=poutputGreyMap->data;
    unsigned i;

    RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap)

    for (i=0; i<pinputGreyMap->size; i++, inP++, outP++) {
        if (*inP<threshold) *outP=0;
        else *outP=*inP;
    }
    return true;
}

/* Command-line version. */
int zeroExecute(unsigned argc,
    const char **argv,
    riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap) {
    riplGrey threshold;

    if (argc<1) {
        riplMessage(itError, "Incorrect number of parameters!\n"
            "Usage: " RIPL_EXENAME " " RIPL_CMDLINE " zero <level>\n");
        return RIPL_PARAMERROR;
    }
    if (!riplArgGet_riplGrey(argv[0], &threshold)) {
        riplMessage(itError, "zero: <level> should be a grey level.\n");
        return RIPL_PARAMERROR;
    }
    if (!zeroApplyOperator(pinputGreyMap, poutputGreyMap, threshold))
        return RIPL_EXECUTEERROR;
    return 1;
}

/* Display help screen. */
const char *zeroHelp(void) {
    return " zero <level>\n\n"
        "   Sets all pixel grey levels below <level> to zero while\n"
        "   maintaining the grey level of pixels equal to or above\n"
        "   the threshold <level>.\n";
}


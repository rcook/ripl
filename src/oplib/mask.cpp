/*
 *		mask.c
 *		Source file for masking operator.
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
#include "mask.h"

/* Internal entrypoint. */
bool maskApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    riplGrey mask) {
    const riplGrey *inP=pinputGreyMap->data;
    riplGrey *outP=poutputGreyMap->data;
    unsigned i;
    RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap)
    for (i=0; i<pinputGreyMap->size; i++, inP++, outP++) *outP=*inP & mask;
    return true;
}

/* Command-line version. */
int maskExecute(unsigned argc, const char **argv,
    riplGreyMap *pinputGreyMap, riplGreyMap *poutputGreyMap) {
    riplGrey mask;
    if (argc<1) {
        riplMessage(itError, "Incorrect number of parameters!\n"
            "Usage: " RIPL_EXENAME " " RIPL_CMDLINE
            " mask <bit-pat>\n");
        return RIPL_PARAMERROR;
    }
    if (!riplArgGet_riplGrey(argv[0], &mask)) {
        riplMessage(itError, "mask: <bit-pat> should be a grey level.\n");
        return RIPL_PARAMERROR;
    }
    if (!maskApplyOperator(pinputGreyMap,
        poutputGreyMap,
        mask))
        return RIPL_EXECUTEERROR;
    return 1;
}

/* Display help screen. */
const char *maskHelp(void) {
    return "mask <bit-pat>\n\n"
        "   Logically ANDs each pixel of the image with the specified\n"
        "   mask <bit-pat>.\n";
}


/*
 *		plane.c
 *		Source file for bit-plane extraction operator.
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
#include "plane.h"

/* Internal entrypoint. */
bool planeApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    riplGrey bitplane) {

    riplGrey mask;
    const riplGrey *inP=pinputGreyMap->data;
    riplGrey *outP=poutputGreyMap->data;
    unsigned i;

    RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap)
    RIPL_VALIDATE(bitplane<=7)
    mask=(riplGrey)(1<<bitplane);
    for (i=0; i<pinputGreyMap->size; i++, inP++, outP++)
        *outP=(riplGrey)((*inP & mask) ? RIPL_MAX_GREY:0);
    return true;
}

/* Command-line version. */
int planeExecute(unsigned argc, const char **argv,
    riplGreyMap *pinputGreyMap, riplGreyMap *poutputGreyMap) {
    riplGrey plane;
    if (argc<1) {
        riplMessage(itError, "Incorrect number of parameters!\n"
            "Usage: " RIPL_EXENAME " " RIPL_CMDLINE
            " plane <bitplane>\n");
        return RIPL_PARAMERROR;
    }
    if (!riplArgGet_riplGrey(argv[0], &plane)) {
        riplMessage(itError,
            "plane: <bitplane> should be a value between 0 and 7.\n");
        return RIPL_PARAMERROR;
    }
    if (plane>7) {
        riplMessage(itError,
            "plane: <bitplane> should be a value between 0 and 7.\n");
        return RIPL_PARAMERROR;
    }
    if (!planeApplyOperator(pinputGreyMap,
        poutputGreyMap,
        plane))
        return RIPL_EXECUTEERROR;
    return 1;
}

/* Display help screen. */
const char *planeHelp(void) {
    return "plane <bitplane>\n\n"
        "   Extracts the <bitplane>th plane of the image by masking\n"
        "   each pixel with 2^<bitplane> (the 0th plane contains the\n"
        "   high-frequency information in the image).\n";
}


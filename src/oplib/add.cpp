/*
 *		add.c
 *		Source file for adding operator.
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
#include "add.h"

/* Internal entrypoint. */
bool addApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    float weight,
    const char *pfileName) {

    riplGreyMap *add_greymap;
    const riplGrey *inP=pinputGreyMap->data, *addP;
    riplGrey *outP=poutputGreyMap->data;
    unsigned i;
    float temp;

    RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap)
    RIPL_VALIDATE(pfileName)

    if (!riplFileExists(pfileName)) {
        riplMessage(itError, "add: Input file %s does not exist!\n");
        return false;
    }
    /* Load input image. */
    add_greymap=riplLoadImage(pfileName);
    if (!add_greymap) {
        riplMessage(itError, "add: Unable to load image file %s!\n"
            "[File error or invalid format]\n", pfileName);
        return false;
    }
    RIPL_VALIDATE_GREYMAP(add_greymap)
    if (add_greymap->cols!=pinputGreyMap->cols ||
        add_greymap->rows!=pinputGreyMap->rows) {
        riplMessage(itError, "add: Images do not have the same dimensions!\n");
        return false;
    }

    /* Images are valid. */
    addP=add_greymap->data;
    for (i=0; i<pinputGreyMap->size; i++, inP++, addP++, outP++) {
        temp=*inP+weight*(*addP);
        if (temp<0) temp=0;
        if (temp>RIPL_MAX_GREY) temp=RIPL_MAX_GREY;
        *outP=(riplGrey)temp;
    }

    /* Deallocate greymap and its data. */
    riplFree(add_greymap->data);
    riplFree(add_greymap);

    return true;
}

/* Command-line version. */
int addExecute(unsigned argc,
    const char **argv,
    riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap) {

    float weight;

    if (argc<2) {
        riplMessage(itError, "Incorrect number of parameters!\n"
            "Usage: " RIPL_EXENAME " " RIPL_CMDLINE " add <weight> <file>\n");
        return RIPL_PARAMERROR;
    }
    if (!riplArgGet_float(argv[0], &weight)) {
        riplMessage(itError, "add: <weight> should be a float.\n");
        return RIPL_PARAMERROR;
    }
    if (!addApplyOperator(pinputGreyMap,
        poutputGreyMap,
        weight,
        argv[1]))
        return RIPL_EXECUTEERROR;
    return 2;
}

/* Display help screen. */
const char *addHelp(void) {
    return " add <weight> <file>\n\n"
        "   Adds a version of the image in <file> weighted by the\n"
        "   factor <weight> to the input image pixel by pixel. The\n"
        "   input image and image in <file> must have the same\n"
        "   pixel dimensions.\n";
}


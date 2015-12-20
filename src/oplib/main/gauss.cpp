/*
 *		gauss.c
 *		Source file for Gaussian smoothing filter.
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
#include "gauss.h"
#include "misc.h"

/* Internal entrypoint. */
bool gaussApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    double variance) {

    unsigned kernel_size;
    double *kernel;
    riplGrey *inP, *outP;
    unsigned i;

    RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap)
    RIPL_VALIDATE(variance>=0)

    /* Generate appropriate Gaussian kernel. */
    miscGenerateGaussian(variance, &kernel_size, &kernel);
    RIPL_VALIDATE(kernel)

    /* Perform 1D convolution in x-direction: i is row number. */
    inP=pinputGreyMap->data();
    outP=poutputGreyMap->data();
    for (i=0; i<pinputGreyMap->height(); i++) {
        miscConvolve1DFP(inP, outP, pinputGreyMap->width(),
            1, kernel, kernel_size);
        inP+=pinputGreyMap->width();		/* Go to next row. */
        outP+=pinputGreyMap->width();
    }

    /* Perform 1D convolution in y-direction: i is column number. */
    inP=poutputGreyMap->data();
    outP=pinputGreyMap->data();
    for (i=0; i<pinputGreyMap->width(); i++) {
        miscConvolve1DFP(inP, outP, pinputGreyMap->height(),
            pinputGreyMap->width(), kernel, kernel_size);
        inP++;								/* Go to next column. */
        outP++;
    }

    /* Swap pointers round so the output's in the right place. */
    pinputGreyMap->swap(*poutputGreyMap);

    /* Deallocate kernel and return. */
    riplFree(kernel);
    return true;
}

/* Command-line version. */
int gaussExecute(unsigned argc,
    const char **argv,
    riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap) {

    double variance;

    if (argc<1) {
        riplMessage(itError, "Incorrect number of parameters!\n"
            "Usage: " RIPL_EXENAME " " RIPL_CMDLINE
            " gaussian <var>\n");
        return RIPL_PARAMERROR;
    }
    if (!riplArgGet_double(argv[0], &variance)) {
        riplMessage(itError, "gauss: <var> should be a double >= 0.\n");
        return RIPL_PARAMERROR;
    }
    if (variance<0) {
        riplMessage(itError, "gauss: <var> should be a double >= 0.\n");
        return RIPL_PARAMERROR;
    }
    if (!gaussApplyOperator(pinputGreyMap,
        poutputGreyMap,
        variance))
        return RIPL_EXECUTEERROR;
    return 1;
}

/* Display help screen. */
const char *gaussHelp(void) {
    return "gaussian <var>\n\n"
        "   Smoothes image by convolving it with a Gaussian kernel\n"
        "   where <var> is the variance of the kernel.\n";
}


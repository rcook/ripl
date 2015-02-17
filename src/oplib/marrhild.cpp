/*
 *		marrhild.c
 *		Source file for Laplacian-of-Gaussian (Marr-Hildreth) edge detection.
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
#include "marrhild.h"
#include "misc.h"

/* Internal entrypoint. */
bool marrhildApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    double variance) {
    unsigned kernel_size;
    double *kernel;
    riplGrey *inP, *outP;
    unsigned i;

    RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap)
    RIPL_VALIDATE(variance>=0)

    /* Generate appropriate 1D Gaussian kernel. */
    miscGenerateGaussian(variance, &kernel_size, &kernel);
    RIPL_VALIDATE(kernel)
    
    /* Differentiate the kernel twice to get the Laplacian. */
    for (i=0; i<kernel_size-1; i++) {
        kernel[i]=kernel[i+1]-kernel[i];
    }
    for (i=0; i<kernel_size-2; i++) {
        kernel[i]=kernel[i+1]-kernel[i];
    }
    kernel_size-=2;

    /* Perform 1D convolution in x-direction: i is row number. */
    inP=pinputGreyMap->data;
    outP=poutputGreyMap->data;
    for (i=0; i<pinputGreyMap->rows; i++) {
        miscConvolve1DFP(inP, outP, pinputGreyMap->cols,
            1, kernel, kernel_size);
        inP+=pinputGreyMap->cols;		/* Move to next ROW in image. */
        outP+=pinputGreyMap->cols;
    }

    /* Perform 1D convolution in y-direction: i is column number. */
    inP=poutputGreyMap->data;
    outP=pinputGreyMap->data;
    for (i=0; i<pinputGreyMap->cols; i++) {
        miscConvolve1DFP(inP, outP,
            pinputGreyMap->rows,
            pinputGreyMap->cols, kernel, kernel_size);
        inP++;						/* Move to next COLUMN in image. */
        outP++;
    }

    /* Swap pointers round so the output's in the right place. */
    outP=pinputGreyMap->data;
    pinputGreyMap->data=poutputGreyMap->data;
    poutputGreyMap->data=outP;

    /* Deallocate kernel and return. */
    riplFree(kernel);
    return true;
}

/* Command-line version. */
int marrhildExecute(unsigned argc,
    const char **argv,
    riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap) {

    double variance;

    if (argc<1) {
        riplMessage(itError, "Incorrect number of parameters!\n"
            "Usage: " RIPL_EXENAME " " RIPL_CMDLINE
            " marrhildreth <var>\n");
        return RIPL_PARAMERROR;
    }
    if (!riplArgGet_double(argv[0], &variance)) {
        riplMessage(itError, "marrhildreth: <var> should be double >= 0.\n");
        return RIPL_PARAMERROR;
    }
    if (variance<0) {
        riplMessage(itError, "marrhildreth: <var> should be double >= 0.\n");
        return RIPL_PARAMERROR;
    }
    if (!marrhildApplyOperator(pinputGreyMap,
        poutputGreyMap,
        variance))
        return RIPL_EXECUTEERROR;
    return 1;
}

/* Display help screen. */
const char *marrhildHelp(void) {
    return "marrhildreth <var>\n\n"
        "   Performs Marr-Hildreth edge detection by convolving the\n"
        "   Laplacian of the Gaussian with the specified variance <var>\n"
        "   with the image and detecting zero crossings (EVENTUALLY).\n";
}


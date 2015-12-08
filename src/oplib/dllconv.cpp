/*
 *		dllconv.c
 *		Source file for dllconvolution of arbitrary 2D integer kernel with image.
 *		Slightly modified syntax for use with Racdll Tcl/Tk interface.
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
#include "dllconv.h"
#include "misc.h"

/* Internal entrypoint. */
bool dllconvApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    unsigned scale,
    bool store_abs,
    unsigned kernel_columns,
    unsigned kernel_rows,
    dllconvKernelType kernel_type,
    void *kernel) {

    RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap)
    RIPL_VALIDATE(kernel_columns>0 && kernel_rows>0 && kernel)

    if (!kernel) return false;
    switch (kernel_type) {
        case dllktint:
            miscConvolve2DInt(pinputGreyMap->data(),
                poutputGreyMap->data(),
                pinputGreyMap->width(),
                pinputGreyMap->height(),
                (int *)kernel,
                kernel_columns, kernel_rows, scale, store_abs);
            return true;
        case dllktdouble:
            miscConvolve2DFP(pinputGreyMap->data(),
                poutputGreyMap->data(),
                pinputGreyMap->width(),
                pinputGreyMap->height(),
                (double *)kernel,
                kernel_columns, kernel_rows, scale, store_abs);
            return true;
        default:
            return false;
    }
}

/* Command-line version. */
int dllconvExecute(unsigned argc,
    const char **argv,
    riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap) {

    unsigned scale, kernel_columns, kernel_rows, kernel_elements, i;
    dllconvKernelType kernel_type=dllktInvalid;
    void *kernel;
    bool store_abs, result;
    
    if (argc<6) {
        riplMessage(itError, "Incorrect number of parameters!\n"
            "Usage: " RIPL_EXENAME " " RIPL_CMDLINE
            " dllconv <type> <scale> <store-abs> <ncols> <nrows> <kernel>\n");
        return RIPL_PARAMERROR;
    }
    if (riplMatch(argv[0], "int", 3)) kernel_type=dllktint;
    if (riplMatch(argv[0], "fp", 2)) kernel_type=dllktdouble;
    if (kernel_type==dllktInvalid) {
        riplMessage(itError, "dllconv: <type> should be 'int' or 'fp'.\n");
        return RIPL_PARAMERROR;
    }
    if (!riplArgGet_unsigned(argv[1], &scale)) {
        riplMessage(itError, "dllconv: <scale> should be an integer >= 1.\n");
        return RIPL_PARAMERROR;
    }
    if (!scale) {
        riplMessage(itError, "dllconv: <scale> should be an integer >= 1.\n");
        return RIPL_PARAMERROR;
    }
    if (!riplArgGet_bool(argv[2], &store_abs)) {
        riplMessage(itError, "dllconv: <store-abs> should be a boolean value.\n");
        return RIPL_PARAMERROR;
    }
    if (!riplArgGet_unsigned(argv[3], &kernel_columns)) {
        riplMessage(itError, "dllconv: <ncols> should be an integer >= 1.\n");
        return RIPL_PARAMERROR;
    }
    if (!kernel_columns) {
        riplMessage(itError, "dllconv: <ncols> should be an integer >= 1.\n");
        return RIPL_PARAMERROR;
    }
    if (!riplArgGet_unsigned(argv[4], &kernel_rows)) {
        riplMessage(itError, "dllconv: <nrows> should be an integer >= 1.\n");
        return RIPL_PARAMERROR;
    }
    if (!kernel_rows) {
        riplMessage(itError, "dllconv: <nrows> should be an integer >= 1.\n");
        return RIPL_PARAMERROR;
    }
    kernel_elements=kernel_columns*kernel_rows;
    if (argc<kernel_elements+5) {
        riplMessage(itError, "Incorrect number of parameters!\n"
            "Usage: " RIPL_EXENAME " " RIPL_CMDLINE
            " dllconv <type> <scale> <store-abs> <ncols> <nrows> <kernel>\n");
        return RIPL_PARAMERROR;
    }
    switch (kernel_type) {
        case dllktint:
            {
                int *ptr;
                kernel=riplCalloc(kernel_elements, sizeof(int));
                ptr=(int *)kernel;
                for (i=0; i<kernel_elements; i++, ptr++) {
                    if (!riplArgGet_int(argv[5+i], ptr)) {
                        riplFree(kernel);
                        riplMessage(itError, "dllconv: <kernel> values should "
                            "be integers.\n");
                        return RIPL_PARAMERROR;
                    }
                }
            }
            break;
        case dllktdouble:
            {
                double *ptr;
                kernel=riplCalloc(kernel_elements, sizeof(double));
                ptr=(double *)kernel;
                for (i=0; i<kernel_elements; i++, ptr++) {
                    if (!riplArgGet_double(argv[5+i], ptr)) {
                        riplFree(kernel);
                        riplMessage(itError, "dllconv: <kernel> values should "
                            "be doubles.\n");
                        return RIPL_PARAMERROR;
                    }
                }
            }
            break;
    }
    result=dllconvApplyOperator(pinputGreyMap,
        poutputGreyMap,
        scale,
        store_abs,
        kernel_columns,
        kernel_rows,
        kernel_type,
        kernel);
    riplFree(kernel);
    if (!result) return RIPL_EXECUTEERROR;
    return kernel_elements+5;
}

/* Return help information. */
const char *dllconvHelp(void) {
    return "dllconv <type> <scale> <store-abs> <ncols> <nrows> <kernel>\n\n"
        "   Variation of the 'conv' function with slightly different syntax.\n"
        "   Implemented for the Racdll Tcl/Tk interface.\n\n"
        "   Performs convolution with specified 2D kernel. <ncols> and <nrows>\n"
        "   specify the dimensions of the kernel while <kernel> are the\n"
        "   <columns>*<rows> elements of the kernel (column index varying\n"
        "   most quickly). <type> should be 'int' or 'fp' indicating of what\n"
        "   type the kernel is. <scale> is unsigned value by which the\n"
        "   convolution sums are scaled, while <store-abs>, a boolean,\n"
        "   determines whether negative convolution sums have zeroes or their\n"
        "   absolute values stored in the final convolution.";
}


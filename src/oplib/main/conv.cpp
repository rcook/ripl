#include "conv.h"

#include "alloc.h"
#include "misc.h"
#include "register.h"

/* Internal entrypoint. */
bool convApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    unsigned kernel_columns,
    unsigned kernel_rows,
    convKernelType kernel_type,
    void *kernel) {
    RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap)
    RIPL_VALIDATE(kernel_columns>0 && kernel_rows>0 && kernel)
    if (!kernel) return false;
    switch (kernel_type) {
        case ktint:
            miscConvolve2DInt(pinputGreyMap->data(),
                poutputGreyMap->data(),
                pinputGreyMap->width(),
                pinputGreyMap->height(),
                (int *)kernel,
                kernel_columns, kernel_rows, 1, false);
            return true;
        case ktdouble:
            miscConvolve2DFP(pinputGreyMap->data(),
                poutputGreyMap->data(),
                pinputGreyMap->width(),
                pinputGreyMap->height(),
                (double *)kernel,
                kernel_columns, kernel_rows, 1, false);
            return true;
        default:
            return false;
    }
}

/* Command-line version. */
int convExecute(unsigned argc, const char **argv,
    riplGreyMap *pinputGreyMap, riplGreyMap *poutputGreyMap) {
    unsigned kernel_columns, kernel_rows, kernel_elements, i;
    convKernelType kernel_type=ktInvalid;
    void *kernel;
    bool result;
    if (argc<4) {
        riplMessage(itError, "Incorrect number of parameters!\n"
            "Usage: " RIPL_EXENAME " " RIPL_CMDLINE
            " conv <type> <ncols> <nrows> <kernel>\n");
        return RIPL_PARAMERROR;
    }
    if (riplMatch(argv[0], "int", 3)) kernel_type=ktint;
    if (riplMatch(argv[0], "fp", 2)) kernel_type=ktdouble;
    if (kernel_type==ktInvalid) {
        riplMessage(itError, "conv: <type> should be 'int' or 'fp'.\n");
        return RIPL_PARAMERROR;
    }
    if (!riplArgGet_unsigned(argv[1], &kernel_columns)) {
        riplMessage(itError, "conv: <ncols> should be an integer >= 1.\n");
        return RIPL_PARAMERROR;
    }
    if (!kernel_columns) {
        riplMessage(itError, "conv: <ncols> should be an integer >= 1.\n");
        return RIPL_PARAMERROR;
    }
    if (!riplArgGet_unsigned(argv[2], &kernel_rows)) {
        riplMessage(itError, "conv: <nrows> should be an integer >= 1.\n");
        return RIPL_PARAMERROR;
    }
    if (!kernel_rows) {
        riplMessage(itError, "conv: <nrows> should be an integer >= 1.\n");
        return RIPL_PARAMERROR;
    }
    kernel_elements=kernel_columns*kernel_rows;
    if (argc<kernel_elements+3) {
        riplMessage(itError, "Incorrect number of parameters!\n"
            "Usage: " RIPL_EXENAME " " RIPL_CMDLINE
            " conv <type> <ncols> <nrows> <kernel>\n");
        return RIPL_PARAMERROR;
    }
    switch (kernel_type) {
        case ktint:
            {
                int *ptr;
                kernel=riplCalloc(kernel_elements, sizeof(int));
                ptr=(int *)kernel;
                for (i=0; i<kernel_elements; i++, ptr++) {
                    if (!riplArgGet_int(argv[3+i], ptr)) {
                        riplFree(kernel);
                        riplMessage(itError, "conv: <kernel> values should "
                            "be integers.\n");
                        return RIPL_PARAMERROR;
                    }
                }
            }
            break;
        case ktdouble:
            {
                double *ptr;
                kernel=riplCalloc(kernel_elements, sizeof(double));
                ptr=(double *)kernel;
                for (i=0; i<kernel_elements; i++, ptr++) {
                    if (!riplArgGet_double(argv[3+i], ptr)) {
                        riplFree(kernel);
                        riplMessage(itError, "conv: <kernel> values should "
                            "be doubles.\n");
                        return RIPL_PARAMERROR;
                    }
                }
            }
            break;
    }
    result=convApplyOperator(pinputGreyMap,
        poutputGreyMap,
        kernel_columns,
        kernel_rows,
        kernel_type,
        kernel);
    riplFree(kernel);
    if (!result) return RIPL_EXECUTEERROR;
    return kernel_elements+3;
}

/* Return help information. */
const char *convHelp(void) {
    return "conv <type> <ncols> <nrows> <kernel>\n\n"
        "   Performs convolution with specified 2D kernel. <ncols> and <nrows>\n"
        "   specify the dimensions of the kernel while <kernel> are the\n"
        "   <columns>*<rows> elements of the kernel (column index varying\n"
        "   most quickly). <type> should be 'int' or 'fp' indicating of what\n"
        "   type the kernel is.\n";
}

OPLIB_REGISTER_OP(
    conv,
    "perform 2D convolution",
    convExecute,
    convHelp);

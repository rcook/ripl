/*
 *		siahe2.c
 *		Source file for extended raised-cosine subimage AHE as described in
 *		an appendix of `Image-Processing Techniques with Application in Image
 *		Restoration' by Richard A. Cook.
 *
 *		Usage: ripl <in-file> <out-file> siahe2 <w-r> <w-c> <d-r> <d-c> <wp>
 *
 *		Version 1.1, last update: 9 May 1998.
 *
 *		History:
 *			9/5/98:		first implemented.
 *
 *		Copyright © 1998, Richard A. Cook.
 */
#include "siahe2.h"

static void do_histogram_eq(riplGrey *pinput,
    unsigned rows,
    unsigned cols,
    void *arg);

/* Internal entrypoint. */
bool siahe2ApplyOperator(riplGreyMap *pinputgrey,
    riplGreyMap *poutputgrey,
    unsigned w_r,
    unsigned w_c,
    unsigned d_r,
    unsigned d_c,
    miscWindowProfile wp) {

    RIPL_VALIDATE_OP_GREYMAPS(pinputgrey, poutputgrey)
    RIPL_VALIDATE(wp==wpCosine || wp==wpTriangular)

    return miscSubimageG(pinputgrey->data(),
        poutputgrey->data(),
        pinputgrey->height(),
        pinputgrey->width(),
        w_r, w_c, d_r, d_c, wp, do_histogram_eq, NULL);
}

/* Command-line version. */
int siahe2Execute(unsigned argc,
    const char **argv,
    riplGreyMap *pinputgrey,
    riplGreyMap *poutputgrey) {

    unsigned w_r, w_c, d_r, d_c;
    miscWindowProfile wp=wpInvalid;

    if (argc<5) {
        riplMessage(itError, "Syntax error!\n"
            "Usage: " RIPL_EXENAME " " RIPL_CMDLINE
            " siahe2 <w-r> <w-c> <d-r> <d-c> <wp>\n");
        return RIPL_PARAMERROR;
    }
    if (riplMatch(argv[4], "cosine", 3)) wp=wpCosine;
    else {
        if (riplMatch(argv[4], "triang", 3)) wp=wpTriangular;
    }
    if (wp==wpInvalid) {
        riplMessage(itError, "siahe2: <wp> should be 'cosine' or 'triang'.\n");
        return RIPL_PARAMERROR;
    }
    if (!riplArgGet_unsigned(argv[0], &w_r)) {
        riplMessage(itError, "siahe2: <w-r> should be an integer > 1.\n");
        return RIPL_PARAMERROR;
    }
    if (w_r>pinputgrey->height()) {
        riplMessage(itError,
            "siahe2: <w-r> should be less than rows in image.\n");
        return RIPL_PARAMERROR;
    }
    if (!riplArgGet_unsigned(argv[1], &w_c)) {
        riplMessage(itError, "siahe2: <w-c> should be an integer > 1.\n");
        return RIPL_PARAMERROR;
    }
    if (w_c>pinputgrey->width()) {
        riplMessage(itError,
            "siahe2: <w-c> should be less than columns in image.\n");
        return RIPL_PARAMERROR;
    }
    if (!riplArgGet_unsigned(argv[2], &d_r)) {
        riplMessage(itError, "siahe2: <d-r> should be an integer > 1.\n");
        return RIPL_PARAMERROR;
    }
    if (d_r>w_r/2) {
        riplMessage(itError,
            "siahe2: <d-r> should be less than half of <w-r>.\n");
        return RIPL_PARAMERROR;
    }
    if (!riplArgGet_unsigned(argv[3], &d_c)) {
        riplMessage(itError, "siahe2: <d-c> should be an integer > 1.\n");
        return RIPL_PARAMERROR;
    }
    if (d_c>w_c/2) {
        riplMessage(itError,
            "siahe2: <d-c> should be less than half of <w-c>.\n");
        return RIPL_PARAMERROR;
    }
    if (!siahe2ApplyOperator(pinputgrey,
        poutputgrey,
        w_r, w_c, d_r, d_c, wp)) return RIPL_EXECUTEERROR;
    return 5;
}

/* Display help screen. */
const char *siahe2Help(void) {
    return "siahe2 <w-r> <w-c> <d-r> <d-c> <wp>\n\n"
        "   Applies subimage-based adaptive histogram equalization based\n"
        "   on the overlapped extended raised-cosine window described in\n"
        "   project report. <w-r> and <w-c> vertical and horizontal size\n"
        "   of window and <d-r> and <d-c> are the overlap between adjacent\n"
        "   subimages. <w-p> is the window profile which can be 'cosine'\n"
        "   for raised-cosine profile or 'triang' for triangular profile.\n";
}

#include <stdio.h>
#ifdef __BORLANDC__
#	pragma argsused
#endif
static void do_histogram_eq(riplGrey *pinput,
    unsigned rows,
    unsigned cols,
    void *arg) {

    miscHistogramEQ(pinput, pinput, rows*cols);
}


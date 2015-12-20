/*
 *		siahe.c
 *		Source file for adaptive histogram equalization operator.
 *
 *		Version 1.1, last update: 10 February 1998.
 *
 *		History:
 *			10/2/98:		modified to use miscRescaleMG.
 *			24/1/98:		introduced RIPL_PARAMERROR.
 *			16/12/97:	updated comments.
 *			27/11/97:	first implemented.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#include <stdio.h>
#include <string.h>
#include "siahe.h"

/* Internal entrypoint. */
bool siaheApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    miscWindowFunction win_func,
    unsigned window_size) {

    unsigned buffer_size=window_size*window_size;
    riplGrey *buffer=(riplGrey *)riplCalloc(buffer_size, sizeof(riplGrey));
    riplMidGrey *temp_output=
        (riplMidGrey *)riplCalloc(poutputGreyMap->size(), sizeof(riplMidGrey));
    double *window=(double *)riplCalloc(window_size, sizeof(double));
    unsigned step=window_size/2,
        xwins, ywins, x, y, im_x, im_y, b, r, index, i, j;

    RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap)
    RIPL_VALIDATE(buffer)
    RIPL_VALIDATE(window)
    RIPL_VALIDATE(temp_output)
    RIPL_VALIDATE(window_size>1)

    /* Calculate number of windows in x- and y-directions. */
    xwins=pinputGreyMap->width()/step;
    ywins=pinputGreyMap->height()/step;

    /* Generate appropriate window. */
    miscGenerateWindowD(win_func, window, window_size);

    /* Set pixels of output image to zero first. */
    memset(temp_output, 0, poutputGreyMap->size()*sizeof(riplMidGrey));

    /* Perform subimage histogram equalizations. */
    for (y=0, im_y=0; im_y<ywins; im_y++, y+=step) {
        b=y+window_size;
        b=b<pinputGreyMap->height() ? b:pinputGreyMap->height();
        for (x=0, im_x=0; im_x<xwins; im_x++, x+=step) {
            r=x+window_size;
            r=r<pinputGreyMap->width() ? r:pinputGreyMap->width();

            /* Copy window area into buffer. */
            for (index=0, i=y; i<b; i++) {
                for (j=x; j<r; j++) {
                    buffer[index++]=pinputGreyMap->data()[i*pinputGreyMap->width()+j];
                }
            }

            /* Perform local histogram equalization. */
            miscHistogramEQ(buffer, buffer, index);

            /* Accumulate local HEs. */
            for (index=0, i=y; i<b; i++) {
                for (j=x; j<r; j++, index++) {
                    *(temp_output+i*pinputGreyMap->width()+j)
                        +=(riplGrey)(window[i-y]*window[j-x]*buffer[index]);
                }
            }
        }
    }

    /* Free the buffer and window blocks. */
    riplFree(buffer);
    riplFree(window);

    /* Rescale image grey levels and copy to output. */
    miscRescaleMG(temp_output, poutputGreyMap->data(), poutputGreyMap->size());

    /* Free buffers and return. */
    riplFree(temp_output);
    return true;
}

/* Command-line version. */
int siaheExecute(unsigned argc, const char **argv,
    riplGreyMap *pinputGreyMap, riplGreyMap *poutputGreyMap) {
    miscWindowFunction window_func=wfInvalid;
    unsigned window_size;
    if (argc<2) {
        riplMessage(itError, "Syntax error!\n"
            "Usage: " RIPL_EXENAME " " RIPL_CMDLINE
            " siahe <win-type> <win-size>\n");
        return RIPL_PARAMERROR;
    }
    if (riplMatch(argv[0], "rect", 3)) window_func=wfRectangular;
    else {
        if (riplMatch(argv[0], "hanning", 3)) window_func=wfHanning;
        else {
            if (riplMatch(argv[0], "hamming", 3)) window_func=wfHamming;
            else {
                if (riplMatch(argv[0], "triang", 3)) window_func=wfTriangular;
            }
        }
    }
    if (window_func==wfInvalid) {
        riplMessage(itError, "siahe: <win-type> should be 'rect', 'hanning', "
            "'hamming' or 'triang'.\n");
        return RIPL_PARAMERROR;
    }
    if (!riplArgGet_unsigned(argv[1], &window_size)) {
        riplMessage(itError, "siahe: <win-size> should be an integer > 1.\n");
        return RIPL_PARAMERROR;
    }
    if (window_size<2) {
        riplMessage(itError, "siahe: <win-size> should be an integer > 1.\n");
        return RIPL_PARAMERROR;
    }
    if (!siaheApplyOperator(pinputGreyMap,
        poutputGreyMap,
        window_func,
        window_size)) return RIPL_EXECUTEERROR;
    return 2;
}

/* Display help screen. */
const char *siaheHelp(void) {
    return "siahe <win-type> <win-size>\n\n"
        "   Applies subimage-based adaptive histogram equalization\n"
        "   to the image. <win-size> is window size (> 1). <win-type>\n"
        "   should be one of 'rect', 'hanning', 'hamming' or 'triang'.\n";
}


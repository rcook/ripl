/*
 *		sine.c
 *		Generates sinusoidally-varying greymap.
 *
 *		Programme part of Ripl test suite.
 *
 *		Ripl Version 1.1, last update: 31 January 1998.
 *
 *		History:
 *			31/1/98:		renamed mathematical constants.
 *			23/1/98:		minor modification: M_PI --> RIPL_PI etc.
 *			21/1/98:		updates in progress.
 *			20/1/98:		first version.
 *
 *		Copyright © 1998, Richard A. Cook.
 */
#include <stdlib.h>
#include <math.h>
#include "ripl.h"

int main(int argc, char **argv) {

    unsigned rcycles, ccycles, i, j;
    float rfreq, cfreq;
    riplGreyMap grey_map;
    riplGrey *pgrey;
    riplBool result;

    /* Read in and check command-line arguments. */
    if (argc!=6) {
        riplMessage(itError,
            "Syntax: sine <rows> <cols> <rfreq> <cfreq> <output-filename>.\n"
            "<rfreq> and <cfreq> are number of whole cycles in y- and\n"
            "x-directions (rows and columns) respectively.\n");
        return EXIT_FAILURE;
    }
    if (!riplArgGet_unsigned(argv[1], &grey_map.rows)) {
        riplMessage(itError, "sine: <rows> should be an unsigned value.\n");
        return EXIT_FAILURE;
    }
    if (!riplArgGet_unsigned(argv[2], &grey_map.cols)) {
        riplMessage(itError, "sine: <cols> should be an unsigned value.\n");
        return EXIT_FAILURE;
    }
    if (!riplArgGet_unsigned(argv[3], &rcycles)) {
        riplMessage(itError, "sine: <rfreq> should be an unsigned value.\n");
        return EXIT_FAILURE;
    }
    if (!riplArgGet_unsigned(argv[4], &ccycles)) {
        riplMessage(itError, "sine: <cfreq> should be an unsigned value.\n");
        return EXIT_FAILURE;
    }

    /* Allocate image. */
    grey_map.size=grey_map.rows*grey_map.cols;
    pgrey=grey_map.data=(riplGrey *)riplCalloc(grey_map.size, sizeof(riplGrey));
    RIPL_VALIDATE_GREYMAP(&grey_map)

    /* Calculate spatial frequencies. */
    rfreq=RIPL_M_TWICE_PI*rcycles/grey_map.rows;
    cfreq=RIPL_M_TWICE_PI*ccycles/grey_map.cols;

    /* Generate image. */
    for (i=0; i<grey_map.rows; i++, pgrey+=grey_map.cols) {
        for (j=0; j<grey_map.cols; j++) {
            pgrey[j]=(riplGrey)(RIPL_MAX_GREY*(sin(rfreq*i+cfreq*j)+1.0)/2.0);
        }
    }

    /* Write image out to specified file. */
    result=riplSaveImage(argv[5], gfPGMBinary, &grey_map);
    if (result) {
        riplMessage(itInfo, "Sinusoidal image successfully saved to %s!\n",
            argv[5]);
    }
    else {
        riplMessage(itError, "Error writing image file %s!\n",
            argv[5]);
    }

    /* Deallocate data and return to system. */
    riplFree(grey_map.data);
    return result ? EXIT_SUCCESS:EXIT_FAILURE;
}

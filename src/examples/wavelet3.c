/*
 *		wavelet3.c
 *		Investigating wavelet decomposition of some data.
 *
 *		Programme part of Ripl test suite.
 *		Uses Ripl Wavelet Transform Toolbox.
 *
 *		WAVELET EXPERIMENT NO. 2
 *		(See Excel spreadsheet for further details.)
 *		Decomposes a 256-element data vector comprising a sine wave into
 *		Daubechies 12-coefficient wavelet basis functions to show the effects
 *		of coefficient quantization which can be used for data compression
 *		and the like.
 *
 *		Ripl Version 1.1, last update: 31 January 1998.
 *
 *		History:
 *			31/1/98:		renamed mathematical constants.
 *			30/1/98:		modified to reflect changes to riplwave.h
 *			26/1/98:		changed filename.
 *			25/1/98:		first version.
 *
 *		Copyright © 1998, Richard A. Cook.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "riplwave.h"

/* Length of data series. */
#define LEN		256
/* Number of cycles of sine wave to put in it. */
#define CYCLES	2

/*
 *	Example threshold levels for wavelet coefficients.
 *			THRESHOLD	NO. BASIS FUNCTIONS
 *				1e-8				197
 *				1e-6				62
 *				1					6
 *				5					2
 */

int main(int argc, char **argv) {

    unsigned i, j, count;
    float data[LEN], wt[LEN], partial[LEN], approx[LEN];
    float freq=2*RIPL_M_PI*CYCLES/LEN;
    float thresh;
    riplwtFilter filter;
    FILE *pfile;

    /* Get/check command-line arguments. */
    if (argc<2) {
        riplMessage(itInfo, "Usage: wavelet3 <filename> <threshold>.\n"
            "Outputs reconstruction of sinusoidal data after\n"
            "thresholding Daubechies wavelet coefficients as\n"
            "specified.\n");
        return EXIT_FAILURE;
    }
    if (!riplArgGet_float(argv[2], &thresh)) {
        riplMessage(itError, "Usage: wavelet3 <filename> <threshold>.\n");
        return EXIT_FAILURE;
    }

    /* Try to open output file. */
    pfile=fopen(argv[1], "wt");
    if (!pfile) {
        riplMessage(itError, "wavelet3: unable to open %s for output.\n",
            argv[1]);
        return EXIT_FAILURE;
    }

    /*
     * Initialize filter coefficients struct.
     * Let's use a 12-coefficient Daubechies wavelet.
     */
    riplwtSetupFilter(ftDaub12, &filter);

    /*
     * Fill our data vector with sine wave. Also initialize vector
     * in which we accumulate the approximation later on.
     */
    for (i=0; i<LEN; i++) {
        wt[i]=data[i]=sin(freq*(float)i);
        approx[i]=0.0;
    }

    /* Perform wavelet transform on data set. */
    riplwt1DWT(wt, LEN, ttForward, NULL, &filter);

    /*
     * Calculate approximation to original data by accumulating weighted
     * sum of basis functions with coefficients greater than threshold
     * value.
     */
    /* Obtain partial data from individual wavelet components. */
    count=0;
    for (i=0; i<LEN; i++) {
        if (fabs(wt[i])>=thresh) {
            count++;
            /* Create a unit vector with 1.0 in relevant position in turn. */
            for (j=0; j<LEN; j++) partial[j]=0.0;
            partial[i]=1.0;
            /*
             * Perform inverse wavelet transform with default filter function.
             * in order to obtain wavelet basis function.
             */
            riplwt1DWT(partial, LEN, ttInverse, NULL, &filter);
            for (j=0; j<LEN; j++) approx[j]+=wt[i]*partial[j];
        }
    }

    /* Write the data out to the output text file. */
    fprintf(pfile, "WAVELET3, 25/1/98\n"
        "Reconstruction with threshold level = %g\n"
        "Using %u out of 256 basis functions.\n", thresh, count);
    fprintf(pfile, "%23s %23s\n", "DATA", "APPROX");
    for (i=0; i<LEN; i++) {
        fprintf(pfile, "%23g %23g\n", data[i], approx[i]);
    }
    fclose(pfile);

    /* Success! */
    return EXIT_SUCCESS;
}

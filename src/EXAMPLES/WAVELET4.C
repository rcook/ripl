/*
 *		wavelet4.c
 *		Investigating wavelet decomposition of some 2D data.
 *
 *		Programme part of Ripl test suite.
 *		Uses Ripl Wavelet Transform Toolbox.
 *
 *		WAVELET EXPERIMENT NO. 4
 *		Two-dimensional wavelet transform example. Demonstration of
 *		reconstruction of a 2D data set using Daubechies 12-coefficient
 *		wavelet bases.
 *
 *		Ripl Version 1.1, last update: 31 January 1998.
 *
 *		History:
 *			31/1/98:		modified to reflect changes to riplwave.h
 *			30/1/98:		modified to reflect changes to riplwave.h
 *			26/1/98:		first version.
 *
 *		Copyright © 1998, Richard A. Cook.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "riplwave.h"

/* Variance of Gaussian. */
#define VAR			4

int main(int argc, char **argv) {

	int i;
	unsigned j, k,
		count;					/* No. of significant bases. */
	float threshold,
		gaussian[32],			/* 1D Gaussian kernel. */
		data[1024],				/* 2D Gaussian surface. */
		wt[1024],				/* Input to and output from wavelet transform. */
		partial[1024],			/* Single wavelet component of reconstruction. */
		approx[1024],			/* Reconstructed version of surface. */
		squared_error,			/* Total squared error. */
		*ptr, *ptr2;
	FILE *pfile;
	riplwtFilter filter;		/* Wavelet filter coefficient struct. */

	/* Get/check command-line arguments. */
	if (argc<2) {
		riplMessage(itInfo, "Usage: wavelet4 <filename> <threshold>.\n"
			"Outputs reconstruction of Gaussian surface after\n"
			"thresholding Daubechies wavelet coefficients as\n"
			"specified.\n");
		return EXIT_FAILURE;
	}
	if (!riplArgGet_float(argv[2], &threshold)) {
		riplMessage(itError, "Usage: wavelet4 <filename> <threshold>.\n");
		return EXIT_FAILURE;
	}

	/* Try to open output file. */
	pfile=fopen(argv[1], "wt");
	if (!pfile) {
		riplMessage(itError, "wavelet4: unable to open %s for output.\n",
			argv[1]);
		return EXIT_FAILURE;
	}

	/* Initialize wavelet filter coefficients. */
	riplwtSetupFilter(ftDaub12, &filter);

	/* Generate 1D Gaussian. */
	for (i=-15; i<15; i++) {
		gaussian[i+15]=exp(-(float)i*i/2/VAR/VAR);
	}

	/* Generate 2D Gaussian from 1D version. */
	ptr=data;
	ptr2=wt;
	for (j=0; j<32; j++) {
		for (k=0; k<32; k++) {
			*ptr=gaussian[j]*gaussian[k];
			*ptr2=*ptr;
			ptr++;
			ptr2++;
		}
	}

	/* Perform wavelet transform. */
	riplwt2DWT(wt, 32, 32, ttForward, NULL, &filter);

	/*
	 * Calculate approximation to original data by accumulating weighted
	 * sum of basis functions with coefficients greater than threshold
	 * value.
	 */
	/* Obtain partial data from individual wavelet components. */
	for (j=0; j<1024; j++) approx[j]=0.0;
	count=0;
	for (j=0; j<1024; j++) {
		if (fabs(wt[j])>=threshold) {
			count++;
			/* Create a unit vector with 1.0 in relevant position in turn. */
			for (k=0; k<1024; k++) partial[k]=0.0;
			partial[j]=1.0;
			/*
			 * Perform inverse wavelet transform with default filter function.
			 * in order to obtain wavelet basis function.
			 */
			riplwt2DWT(partial, 32, 32, ttInverse, NULL, &filter);
			for (k=0; k<1024; k++) approx[k]+=wt[j]*partial[k];
		}
	}

	/* Calculate total squared error. */
	squared_error=0.0;
	for (j=0; j<1024; j++) {
		squared_error+=(data[j]-approx[j])*(data[j]-approx[j]);
	}

	/* Write approximation results out to file. */
	fprintf(pfile, "WAVELET4, 26/1/98.\n"
		"Approximation to Gaussian surface using DAUB12 wavelets.\n"
		"Threshold = %e.\n"
		"Total squared error = %e.\n"
		"%u of 1024 basis functions used.\n\n",
		threshold, squared_error, count);
	ptr=approx;
	for (j=0; j<32; j++) {
		for (k=0; k<32; k++) {
			fprintf(pfile, " %23e", *ptr++);
		}
		fprintf(pfile, "\n");
	}
	fclose(pfile);

	/* Finished successfully! */
	return EXIT_SUCCESS;
}

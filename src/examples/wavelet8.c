/*
 *		wavelet8.c
 *		Investigating noise removal using wavelet decomposition.
 *
 *		Programme part of Ripl test suite.
 *		Uses Ripl Wavelet Transform Toolbox.
 *
 *		WAVELET EXPERIMENT NO. 8
 *		Takes an input file, fiddles some coefficients and writes output.
 *
 *		Ripl Version 1.1, last update: 3 February 1998.
 *
 *		History:
 *			3/2/98:		first version.
 *
 *		Copyright © 1998, Richard A. Cook.
 */
#include <stdio.h>
#include "riplwave.h"

int main(int argc, char **argv) {

	/*
	 * This array contains the indices of the coefficients we DON'T
	 * want truncating.
	 */
	const unsigned long fiddle_coeff[]={4, 5, 6, 7, 8, 9, 10,
		11, 12, 13, 14, 15, 16, 20};
	const unsigned n_fiddle_coeffs=sizeof(fiddle_coeff)/sizeof(*fiddle_coeff);
	FILE *pfile1, *pfile2;
	unsigned long length, i, last, j;
	float temp, *vector;
	riplwtFilter filter;

	/* Get/check command-line arguments. */
	if (argc!=3) {
		riplMessage(itInfo,
			"Usage: wavelet8 <in-file> <out-file>.\n");
		return EXIT_FAILURE;
	}
	if (!riplFileExists(argv[1])) {
		riplMessage(itError, "wavelet8: specified input file does not exist.\n");
		return EXIT_FAILURE;
	}

	/* See if we can open output file. */
	pfile1=fopen(argv[2], "wt");
	if (!pfile1) {
		riplMessage(itError, "Cannot open %s for output.\n", argv[3]);
		return EXIT_FAILURE;
	}

	/* Open input file. */
	pfile2=fopen(argv[1], "rt");
	if (!pfile2) {
		riplMessage(itError, "Cannot open %s for input.\n", argv[1]);
		fclose(pfile1);
		remove(argv[3]);
		return EXIT_FAILURE;
	}

	/* Find out how many values are in it by scanning through once. */
	for (length=0; !feof(pfile2); ) {
		if (fscanf(pfile2, "%f", &temp)!=EOF) length++;
	}
	if (!RIPL_IS_POWER_OF_2(length)) {
		riplMessage(itInfo, "No. of values must be integer power of two.\n");
		fclose(pfile2);
		return EXIT_FAILURE;
	}

	/* Allocate vector to store input values in. */
	vector=(float *)riplCalloc(length, sizeof(float));
	RIPL_VALIDATE(vector)
	
	/* Rewind file pointer and read values in. */
	fseek(pfile2, 0L, SEEK_SET);
	for (i=0; i<length; i++) fscanf(pfile2, "%f", &vector[i]);
	fclose(pfile2);

	/* Set up wavelet filter. */
	riplwtSetupFilter(ftDaub4, &filter);

	/* Perform wavelet transform using default filtering function. */
	riplwt1DWT(vector, length, ttForward, NULL, &filter);

	/* Do the fiddling. */
	last=0;
	for (i=0; i<n_fiddle_coeffs; i++) {
		for (j=last; j<fiddle_coeff[i]; j++) vector[j]=0.0;
		last=j+1;
	}
	for (i=last; i<length; i++) vector[i]=0.0;

	/* Perform inverse wavelet transform using default filtering function. */
	riplwt1DWT(vector, length, ttInverse, NULL, &filter);

	/* Write results to output file. */
	for (i=0; i<length; i++) {
		fprintf(pfile1, "%20e\n", vector[i]);
	}
	fclose(pfile1);

	/* Tidy up. */
	riplFree(vector);
	return EXIT_SUCCESS;
}


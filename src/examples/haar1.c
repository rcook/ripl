/*
 *		haar1.c
 *		Investigating Haar wavelet decomposition of some 1D data.
 *
 *		Programme part of Ripl test suite.
 *		Uses Ripl Wavelet Transform Toolbox.
 *
 *		HAAR WAVELET EXPERIMENT NO. 1
 *		Generate some Haar wavelet basis functions.
 *
 *		Ripl Version 1.1, last update: 23 February 1998.
 *
 *		History:
 *			23/2/98:		'ftHaar' renamed 'ftHaar2'.
 *			30/1/98:		first version.
 *
 *		Copyright © 1998, Richard A. Cook.
 */
#include <stdio.h>
#include <stdlib.h>
#include "riplwave.h"

#define LEN			16

#ifdef __BORLANDC__
#	pragma argsused
#endif
int main(int argc, char **argv) {

	float data[LEN];
	riplwtFilter filter;
	unsigned i, j;
	FILE *pfile;

	/* Check command-line arguments. */
	if (argc<2) {
		riplMessage(itError,
			"Usage: haar1 <filename>.\n");
		return EXIT_FAILURE;
	}
	RIPL_VALIDATE(argv[1])

	/* Initialize wavelet filter coefficients. */
	riplwtSetupFilter(ftHaar2, &filter);

	/* Write output to specified file. */
	pfile=fopen(argv[1], "wt");
	RIPL_VALIDATE(pfile)

	for (i=0; i<LEN; i++) {

		/* Create relevant unit vector. */
		for (j=0; j<LEN; j++) data[j]=0.0;
		data[i]=1.0;

		/* Perform inverse wavelet transform to obtain basis function. */
		riplwt1DWT(data, LEN, ttInverse, NULL, &filter);

		/* Write it out to the file. */
		fprintf(pfile, "basis %3u:", i);
		for (j=0; j<LEN; j++) fprintf(pfile, " %20e", data[j]);
		fprintf(pfile, "\n");
	}
	fclose(pfile);

	/* Success! */
	return EXIT_SUCCESS;
}


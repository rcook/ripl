/*
 *		wavelet2.c
 *		Investigating wavelet decomposition of some data.
 *
 *		Programme part of Ripl test suite.
 *		Uses Ripl Wavelet Transform Toolbox.
 *
 *		WAVELET EXPERIMENT NO. 1
 *		(See Excel spreadsheet for further details.)
 *		Decomposes a 16-element data vector into Daubechies 12-coefficient
 *		wavelet basis functions to show the effects of coefficient quantization
 *		which can be used for data compression and the like.
 *
 *		Ripl Version 1.1, last update: 31 January 1998.
 *
 *		History:
 *			31/1/98:		modified to reflect changes to riplwave.h
 *			30/1/98:		modified to reflect changes to riplwave.h
 *			26/1/98:		changed filename.
 *			25/1/98:		first version.
 *
 *		Copyright © 1998, Richard A. Cook.
 */
#include <stdio.h>
#include <stdlib.h>
#include "riplwave.h"

#define LEN		16

#ifdef __BORLANDC__
#	pragma argsused
#endif
int main(int argc, char **argv) {

	unsigned i, j;
	float data[LEN], wt[LEN], partial[LEN][LEN];
	riplwtFilter filter;
	FILE *pfile;

	/*
	 * Initialize filter coefficients struct.
	 * Let's use a 12-coefficient Daubechies wavelet.
	 */
	riplwtSetupFilter(ftDaub12, &filter);

	/* Fill our data vector with some interesting (!) data. */
	for (i=0; i<LEN; i++) wt[i]=data[i]=(float)i;

	/* Obtain partial data from individual wavelet components. */
	for (i=0; i<LEN; i++) {
		/* Create a unit vector with 1.0 in each position in turn. */
		for (j=0; j<LEN; j++) partial[i][j]=0.0;
		partial[i][i]=1.0;

		/* Perform inverse wavelet transform with default filter function. */
		riplwt1DWT(partial[i], LEN, ttInverse, NULL, &filter);
	}

	/* Perform wavelet transform on data set. */
	riplwt1DWT(wt, LEN, ttForward, NULL, &filter);

	/* Write the data out to a text file. */
	pfile=fopen("daub12_1.txt", "wt");
	fprintf(pfile, "%23s %23s", "DATA", "COEFFS");
	for (i=0; i<LEN; i++) fprintf(pfile, "                    e%3u", i);
	fprintf(pfile, "\n");
	for (i=0; i<LEN; i++) {
		fprintf(pfile, "%23g %23g", data[i], wt[i]);
		for (j=0; j<LEN; j++) {
			fprintf(pfile, " %23g", partial[j][i]);
		}
		fprintf(pfile, "\n");
	}
	fclose(pfile);

	return EXIT_SUCCESS;
}

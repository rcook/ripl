/*
 *		wavelet7.c
 *		Investigating noise removal using wavelet decomposition.
 *
 *		Programme part of Ripl test suite.
 *		Uses Ripl Wavelet Transform Toolbox.
 *
 *		WAVELET EXPERIMENT NO. 7
 *		Takes an input file as an argument and outputs a cleaned-up version.
 *
 *		Ripl Version 1.1, last update: 23 February 1998.
 *
 *		History:
 *			23/2/98:		'ftHaar' renamed 'ftHaar2'.
 *			3/2/98:		first version.
 *
 *		Copyright © 1998, Richard A. Cook.
 */
#include <stdio.h>
#include <string.h>
#include "riplwave.h"

int main(int argc, char **argv) {

	riplwtFilterType filter_type=ftInvalid;
	FILE *pfile1, *pfile2;
	unsigned long length, i;
	float temp, *vector;
	riplwtFilter filter;

	/* Get/check command-line arguments. */
	if (argc!=4) {
		riplMessage(itInfo,
			"Usage: wavelet7 <in-file> <wave-type> <out-file>.\n");
		return EXIT_FAILURE;
	}
	if (!riplFileExists(argv[1])) {
		riplMessage(itError, "wavelet7: specified input file does not exist.\n");
		return EXIT_FAILURE;
	}
	if (!strcmp(argv[2], "Daub4")) filter_type=ftDaub4;
	if (!strcmp(argv[2], "Daub12")) filter_type=ftDaub12;
	if (!strcmp(argv[2], "Daub4")) filter_type=ftDaub20;
	if (!strcmp(argv[2], "Haar")) filter_type=ftHaar2;
	if (filter_type==ftInvalid) {
		riplMessage(itError,
			"Usage: wavelet7 <in-file> <wave-type> <out-file>.\n");
		return EXIT_FAILURE;
	}

	/* See if we can open output file. */
	pfile1=fopen(argv[3], "wt");
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
	riplwtSetupFilter(filter_type, &filter);

	/* Perform wavelet transform using default filtering function. */
	riplwt1DWT(vector, length, ttForward, NULL, &filter);

	/* Write results to output file. */
	fprintf(pfile1, RIPL_APPNAME " Version " RIPL_VERSION "\n"
		"WAVELET7: Wavelet-Based Noise Removal\n"
		"Image: %s\n"
		"Dimensions: %lux%lu (%lu pixels)\n\n",
		argv[1], length, 1, length);
	for (i=0; i<length; i++) {
		fprintf(pfile1, "%5lu %20e\n", i, vector[i]);
	}
	fclose(pfile1);

	/* Tidy up. */
	riplFree(vector);
	return EXIT_SUCCESS;
}


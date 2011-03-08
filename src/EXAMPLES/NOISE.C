/*
 *		noise.c
 *		Adds salt-and-pepper noise to PGM image.
 *
 *		<in-file>				input file.
 *		<out-file>				output file.
 *		<n-pts>					(approx. )number of salt-and-pepper noise spots to
 *									add.
 *
 *		Uses Ripl Random-Number Generator Toolbox.
 *
 *		Programme part of Ripl test suite.
 *
 *		Ripl Version 1.1, last update: 10 May 1998.
 *
 *		History:
 *			10/5/98:			first version.
 *
 *		Copyright © 1998, Richard A. Cook.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ripl.h"
#include "riplrand.h"

int main(int argc, char **argv) {

	riplGreyMap *pgreymap;
	unsigned long n, i, index;
	long seed;
	riplBool result;

	if (argc!=4) {
		riplMessage(itError, "Usage: noise <in-file> <out-file> <n-pts>\n");
		return EXIT_FAILURE;
	}
	if (!riplArgGet_long(argv[3], &seed)) {
		riplMessage(itError, "noise: <n-pts> should be an unsigned value.\n");
		return EXIT_FAILURE;
	}
	if (seed<1) {
		riplMessage(itError, "noise: <n-pts> should be >=1.\n");
		return EXIT_FAILURE;
	}
	n=(unsigned long)seed;
	if (!riplFileExists(argv[1])) {
		riplMessage(itError, "File %s does not exist!\n", argv[1]);
		return EXIT_FAILURE;
	}
	pgreymap=riplLoadImage(argv[1]);
	if (!pgreymap) {
		riplMessage(itError, "Could not read file %s!\n", argv[1]);
		return EXIT_FAILURE;
	}
	/* Seed the random-number generator using the system clock. */
	time((time_t *)(&seed));
	for (i=0; i<n; i++) {
		index=riplrandRan1(&seed)*(pgreymap->rows-1)*pgreymap->cols+
			riplrandRan1(&seed)*(pgreymap->cols-1);
		if (i%2) pgreymap->data[index]=RIPL_MAX_GREY;
		else pgreymap->data[index]=0;
	}
	/* Write image out to specified file. */
	result=riplSaveImage(argv[2], gfPGMBinary, pgreymap);
	if (result) {
		riplMessage(itInfo, "Noisy image successfully saved to %s!\n",
			argv[2]);
	}
	else {
		riplMessage(itError, "Error writing image file %s!\n",
			argv[2]);
	}
	riplFree(pgreymap->data);
	riplFree(pgreymap);
	return result ? EXIT_SUCCESS:EXIT_FAILURE;
}


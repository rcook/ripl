/*
 *		wavelet.c
 *		Investigating wavelet decomposition of some data.
 *
 *		Programme part of Ripl test suite.
 *		Uses Ripl Wavelet Transform Toolbox.
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

#define ROWS		8
#define COLS		8
#define SIZE		(ROWS*COLS)

#ifdef __BORLANDC__
#	pragma argsused
#endif
int main(int argc, char **argv) {

    unsigned long i;
    float data[SIZE];				/* Our real data set. */
    riplwtFilter filter;			/* Wavelet filter coefficients. */
    FILE *pfile;

    /*
     * Initialize filter coefficients struct.
     * Let's use a 12-coefficient Daubechies wavelet.
     */
    riplwtSetupFilter(ftDaub12, &filter);

    /* Fill our data vector with some interesting (!) data. */
    for (i=0; i<SIZE; i++) data[i]=(float)i;

    /* Perform forward wavelet transform with default filter function. */
    riplwt2DWT(data, ROWS, COLS, ttForward, NULL, &filter);

    /* Perform inverse transform---should have original data at end. */
    riplwt2DWT(data, ROWS, COLS, ttInverse, NULL, &filter);

    /* Write the data out to a text file. */
    pfile=fopen("output.txt", "wt");
    for (i=0; i<SIZE; i++) fprintf(pfile, "%20e\n", data[i]);
    fclose(pfile);

    return EXIT_SUCCESS;
}

/*
 *		hist.c
 *		Tabulates grey-level histogram of portable grey to text file.
 *
 *		Programme part of Ripl test suite.
 *
 *		Ripl Version 1.1, last update: 1 May 1998.
 *
 *		History:
 *			1/5/98:		first version.
 *
 *		Copyright © 1998, Richard A. Cook.
 */
#include <stdio.h>
#include <stdlib.h>
#include "ripl.h"

int main(int argc, char **argv) {

    riplGreyMap *pgreymap;
    FILE *outf;
    unsigned long bin[RIPL_GREY_LEVELS], i;
    unsigned j;

    if (argc!=3) {
        riplMessage(itError, "Usage: hist <pgm-file> <text-file>\n");
        return EXIT_FAILURE;
    }

    if (!riplFileExists(argv[1])) {
        riplMessage(itError, "File %s does not exist!\n", argv[1]);
        return EXIT_FAILURE;
    }
    outf=fopen(argv[2], "wt");
    if (!outf) {
        riplMessage(itError, "Unable to open %s for output!\n", argv[2]);
        return EXIT_FAILURE;
    }

    pgreymap=riplLoadImage(argv[1]);
    if (!pgreymap) {
        riplMessage(itError, "Could not read file %s!\n", argv[1]);
        fclose(outf);
        return EXIT_FAILURE;
    }

    /* Initialize histogram bins to zero. */
    for (i=0; i<RIPL_GREY_LEVELS; i++) bin[i]=0;
    /* Calculate histogram. */
    for (j=0; j<pgreymap->size; j++) {
        bin[pgreymap->data[j]]++;
    }
    /* Write histogram to file. */
    for (i=0; i<RIPL_GREY_LEVELS; i++) {
        fprintf(outf, "%4u %10lu\n", i, bin[i]);
    }
    fclose(outf);
    riplFree(pgreymap->data);
    riplFree(pgreymap);
    return EXIT_SUCCESS;
}

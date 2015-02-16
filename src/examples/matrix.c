/*
 *		matrix.c
 *		Loads portable greymap in and writes the grey levels to
 *		specified text file.
 *
 *		Written by Richard A. Cook, 16/2/98.
 */
#include <stdio.h>
#include <stdlib.h>
#include "ripl.h"

int main(int argc, char **argv) {

	riplGreyMap *pgreymap;
	FILE *outf;
	unsigned long r, c;
	riplGrey *ptr;

	if (argc!=3) {
		riplMessage(itError, "Usage: matrix <pgm-file> <text-file>\n");
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
	ptr=pgreymap->data;
	for (r=0; r<pgreymap->rows; r++) {
		for (c=0; c<pgreymap->cols; c++) {
			fprintf(outf, " %4u", *ptr++);
		}
		fprintf(outf, "\n");
	}
	fclose(outf);
	riplFree(pgreymap->data);
	riplFree(pgreymap);
	return EXIT_SUCCESS;
}

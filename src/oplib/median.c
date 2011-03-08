/*
 *		median.c
 *		Source file for median filtering.
 *
 *		Version 1.1, last update: 24 January 1998.
 *
 *		History:
 *			24/1/98:		introduced RIPL_PARAMERROR.
 *			16/12/97:	updated comments.
 *			27/11/97:	first implemented.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#include <stdlib.h>
#include <string.h>
#include "median.h"

/* Prototypes of static functions. */
static int riplGrey_compare(const void *value1, const void *value2);

/* Internal entrypoint. */
riplBool medianApplyOperator(riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap,
	unsigned window_size) {

	unsigned buffer_length=window_size*window_size,
		median=(buffer_length-1)/2,
		neigh1=(window_size-1)/2,
		neigh2=window_size-neigh1,
		r, c, i, j, index, base;
	const riplGrey *inP;
	riplGrey *outP;
	riplGrey *buffer=(riplGrey *)riplCalloc(buffer_length, sizeof(riplGrey));

	RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap)
	RIPL_VALIDATE(window_size>0)
	RIPL_VALIDATE(buffer)

	/* Set image to white to frame it. */
	outP=poutputGreyMap->data;
	memset(poutputGreyMap->data, RIPL_MAX_GREY, poutputGreyMap->size);

	/* Perform median filtering. */
	inP=pinputGreyMap->data;
	outP=poutputGreyMap->data+neigh1*pinputGreyMap->cols;
	for (r=neigh1; r<=pinputGreyMap->rows-neigh2; r++) {
		for (c=neigh1; c<=pinputGreyMap->cols-neigh2; c++) {
			index=0;
			base=0;
			for (i=r-neigh1; i<r+neigh2; i++) {
				for (j=c-neigh1; j<c+neigh2; j++) {
					buffer[index++]=*(inP+base+j);
				}
				base+=pinputGreyMap->cols;
			}
			qsort((void *)buffer, buffer_length,
				sizeof(riplGrey), riplGrey_compare);
			*(outP+c)=buffer[median];
		}
		inP+=pinputGreyMap->cols;
		outP+=pinputGreyMap->cols;
	}
	riplFree(buffer);
	return true;
}

/* Compare two bytes. */
static int riplGrey_compare(const void *value1, const void *value2) {
	if (*(riplGrey *)value1<*(riplGrey *)value2) return -1;
	if (*(riplGrey *)value1>*(riplGrey *)value2) return 1;
	return 0;
}

/* Command-line version. */
int medianExecute(unsigned argc, const char **argv,
	riplGreyMap *pinputGreyMap, riplGreyMap *poutputGreyMap) {
	unsigned window_size;
	if (argc<1) {
		riplMessage(itError, "Incorrect number of parameters!\n"
			"Usage: " RIPL_EXENAME " " RIPL_CMDLINE
			" median <win-size>\n");
		return RIPL_PARAMERROR;
	}
	if (!riplArgGet_unsigned(argv[0], &window_size)) {
		riplMessage(itError, "median: <win-size> should be an integer >= 1.\n");
		return RIPL_PARAMERROR;
	}
	if (window_size<1) {
		riplMessage(itError, "median: <win-size> should be an integer >= 1.\n");
		return RIPL_PARAMERROR;
	}
	if (!medianApplyOperator(pinputGreyMap,
		poutputGreyMap,
		window_size))
		return RIPL_EXECUTEERROR;
	return 1;
}

/* Display help screen. */
const char *medianHelp(void) {
	return "median <win-size>\n\n"
		"   Applies a median filter to the image using the specified\n"
		"   window size <win-size>.\n";
}


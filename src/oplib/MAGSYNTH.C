/*
 *		magsynth.c
 *		Source file for magnitude-only synthesis operator.
 *
 *		Version 1.1, last update: 16 February 1998.
 *
 *		History:
 *			16/2/98:		first implemented.
 *
 *		Copyright © 1998, Richard A. Cook.
 */
#include <math.h>
#include <stdio.h>
#include "magsynth.h"
#include "riplfft.h"
#include "misc.h"

/* Internal entrypoint. */
riplBool magsynthApplyOperator(riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap) {

	float *data, *nyquist, *ptr;
	unsigned long i;
	riplGrey *pgrey;
	float val;

	RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap)
	RIPL_VALIDATE_IS_POWER_OF_2(pinputGreyMap->rows)
	RIPL_VALIDATE_IS_POWER_OF_2(pinputGreyMap->cols)

	/* Allocate FFT input matrices. */
	data=(float *)riplCalloc(pinputGreyMap->size, sizeof(float));
	RIPL_VALIDATE(data)
	nyquist=(float *)riplCalloc(pinputGreyMap->rows<<1, sizeof(float));
	RIPL_VALIDATE(nyquist)

	/* Copy grey levels into FFT input matrix. */
	ptr=data;
	pgrey=pinputGreyMap->data;
	for (i=pinputGreyMap->size; i; i--, ptr++, pgrey++) {
		ptr[0]=(float)pgrey[0];
	}

	/* Calculate 2D FFT of grey levels. */
	riplfftReal2DFT(data,
		nyquist,
		pinputGreyMap->rows,
		pinputGreyMap->cols,
		ttForward);

	/* Replace coefficients by their magnitudes. */
	ptr=data;
	for (i=pinputGreyMap->size; i; i-=2, ptr+=2) {
		val=sqrt(ptr[0]*ptr[0]+ptr[1]*ptr[1]);
		ptr[0]=val;
		ptr[1]=0.0;
	}

	/* Calculate inverse FFT. */
	riplfftReal2DFT(data,
		nyquist,
		pinputGreyMap->rows,
		pinputGreyMap->cols,
		ttInverse);

	/* Scale output to grey levels. */
	miscRescaleF(data, poutputGreyMap->data, poutputGreyMap->size);

	/* Deallocate input matrices. */
	riplFree(nyquist);
	riplFree(data);
	return true;
}

/* Command-line execution version. */
#ifdef __BORLANDC__
#	pragma argsused
#endif
int magsynthExecute(unsigned argc,
	const char **argv,
	riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap) {

	RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap)

	/* Ensure image is of suitable dimensions. */
	if (!RIPL_IS_POWER_OF_2(pinputGreyMap->rows)
		|| !RIPL_IS_POWER_OF_2(pinputGreyMap->cols)) {
		riplMessage(itError, "magsynth: Number of rows and columns in input\n"
			"image must be integer powers of 2.\n");
		return RIPL_USERERROR;
	}

	/* Apply operator. */
	if (!magsynthApplyOperator(pinputGreyMap,
		poutputGreyMap))
		return RIPL_EXECUTEERROR;
	return 0;
}

/* Display help screen. */
const char *magsynthHelp(void) {
	return " magsynth\n\n"
		"   Fourier-transforms input image and performs magnitude-only\n"
		"   synthesis by discarding the phase content of the Fourier\n"
		"   coefficients\n";
}


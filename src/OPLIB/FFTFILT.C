/*
 *		fftfilt.c
 *		Source file for FFT-based filtering operator.
 *
 *		Version 1.1, last update: 10 February 1998.
 *
 *		History:
 *			10/2/98:		modified to use miscRescaleF.
 *			2/2/98:		improved error messages.
 *			24/1/98:		slight modifications.
 *			21/1/98:		slight modifications.
 *			19/1/98:		first implemented.
 *
 *		Copyright © 1998, Richard A. Cook.
 */
#include <math.h>
#include <stdio.h>
#include "fftfilt.h"
#include "riplfft.h"
#include "misc.h"

/* Locally used macros. */
#define MULTIPLY_REAL(a, b, c, d)	((a)*(c)-(b)*(d))
#define MULTIPLY_IMAG(a, b, c, d)	((a)*(d)+(b)*(c))

/* Internal entrypoint. */
#ifdef __BORLANDC__
#	pragma argsused
#endif
riplBool fftfiltApplyOperator(riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap,
	float *pfreqResp,
	riplBool is_complex) {

	float *data, *nyquist, *ptr1, *ptr2, *ptr3, *ptr4;
	unsigned long i, j;
	riplGrey *pgrey;

	RIPL_VALIDATE(pfreqResp)
	RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap)

	/* Allocate FFT input matrices. */
	data=(float *)riplCalloc(pinputGreyMap->size, sizeof(float));
	RIPL_VALIDATE(data)
	nyquist=(float *)riplCalloc(pinputGreyMap->rows<<1, sizeof(float));
	RIPL_VALIDATE(nyquist)

	/* Copy grey levels into FFT input matrix. */
	ptr1=data;
	pgrey=pinputGreyMap->data;
	for (i=pinputGreyMap->size; i; i--) *ptr1++=(float)(*pgrey++);

	/* Calculate 2D FFT of grey levels. */
	riplfftReal2DFT(data,
		nyquist,
		pinputGreyMap->rows,
		pinputGreyMap->cols,
		ttForward);

	/* Apply filter by multiplying by frequency response. */
	ptr1=data;
	ptr2=data+pinputGreyMap->size;
	ptr3=pfreqResp;
	ptr4=nyquist;
	if (is_complex) {
		for (i=0; i<=(pinputGreyMap->rows>>1); i++) {
			for (j=0; j<pinputGreyMap->cols; j+=2) {
				if (i==0 || i==(pinputGreyMap->rows>>1)) {
					ptr1[j]=MULTIPLY_REAL(ptr1[j], ptr1[j+1], ptr3[0], ptr3[1]);
					ptr1[j+1]=MULTIPLY_IMAG(ptr1[j], ptr1[j+1], ptr3[0], ptr3[1]);
				}
				else {
					ptr1[j]=MULTIPLY_REAL(ptr1[j], ptr1[j+1], ptr3[0], ptr3[1]);
					ptr2[j+1]=-(ptr1[j+1]=MULTIPLY_IMAG(ptr1[j],
						ptr1[j+1], ptr3[0], ptr3[1]));
					ptr2[j]=MULTIPLY_REAL(ptr2[j], ptr2[j+1], ptr3[0], ptr3[1]);
				}
				ptr3+=2;
			}
			ptr4[0]=MULTIPLY_REAL(ptr4[0], ptr4[1], ptr3[0], ptr3[1]);
			ptr4[1]=MULTIPLY_IMAG(ptr4[0], ptr4[1], ptr3[0], ptr3[1]);
			ptr1+=pinputGreyMap->cols;
			ptr2-=pinputGreyMap->cols;
			ptr3+=2;
			ptr4+=2;
		}
	}
	else {
		for (i=0; i<=(pinputGreyMap->rows>>1); i++) {
			for (j=0; j<pinputGreyMap->cols; j+=2) {
				if (i==0 || i==(pinputGreyMap->rows>>1)) {
					ptr1[j]*=ptr3[0];
					ptr1[j+1]*=ptr3[0];
				}
				else {
					ptr1[j]*=ptr3[0];
					ptr2[j+1]=-(ptr1[j+1]*=ptr3[0]);
					ptr2[j]*=ptr3[0];
				}
				ptr3++;
			}
			ptr4[0]*=ptr3[0];
			ptr4[1]*=ptr3[0];
			ptr1+=pinputGreyMap->cols;
			ptr2-=pinputGreyMap->cols;
			ptr3++;
			ptr4+=2;
		}
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
int fftfiltExecute(unsigned argc,
	const char **argv,
	riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap) {

	float *freq_resp;
	unsigned long i, len;
	riplBool is_complex, result;
	FILE *pfile;
	int scan_result;

	RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap)

	if (argc<2) {
		riplMessage(itError, "Incorrect number of parameters!\n"
			"Usage: " RIPL_EXENAME " " RIPL_CMDLINE
			" fftfilt <resp-file> <is-complex>\n");
		return RIPL_PARAMERROR;
	}
	if (!riplFileExists(argv[0])) {
		riplMessage(itError, "Cannot find Frequency response file %s!\n",
			argv[0]);
		return RIPL_USERERROR;
	}
	if (!riplArgGet_riplBool(argv[1], &is_complex)) {
		riplMessage(itError, "fftfilt: <is-complex> should be boolean.\n");
		return RIPL_PARAMERROR;
	}

	/* Ensure image is of suitable dimensions. */
	if (!RIPL_IS_POWER_OF_2(pinputGreyMap->rows)
		|| !RIPL_IS_POWER_OF_2(pinputGreyMap->cols)) {
		riplMessage(itError, "fftfilt: Number of rows and columns in input\n"
			"image must be integer powers of 2.\n");
		return RIPL_USERERROR;
	}

	/* Load frequency response from file. */
	len=((pinputGreyMap->rows>>1)+1)*((pinputGreyMap->cols>>1)+1);
	if (is_complex) len<<=1;
	freq_resp=(float *)riplCalloc(len, sizeof(float));
	RIPL_VALIDATE(freq_resp)

	/* Read frequency response in from specified file. */
	pfile=fopen(argv[0], "rt");
	RIPL_VALIDATE(pfile)
	for (i=0; i<len; i++) {
		scan_result=fscanf(pfile, "%f", &freq_resp[i]);
		if (scan_result!=1) {
			fclose(pfile);
			riplFree(freq_resp);
			if (scan_result==EOF) {
				if (is_complex) {
					riplMessage(itError,
						"Insufficient values in frequency response file %s!\n"
						"File should contain 2*(%lu/2+1)*(%lu/2+1) = %lu values.\n",
						argv[0],
						pinputGreyMap->rows,
						pinputGreyMap->cols,
						len);
				}
				else {
					riplMessage(itError,
						"Insufficient values in frequency response file %s!\n"
						"File should contain (%lu/2-1)*(%lu/2-1) = %lu values.\n",
						argv[0],
						pinputGreyMap->rows,
						pinputGreyMap->cols,
						len);
				}
				return RIPL_USERERROR;
			}
			riplMessage(itError, "Error reading frequency response file %s!\n"
				"[File error]\n", argv[0]);
			return RIPL_EXECUTEERROR;
		}
	}
	fclose(pfile);

	/* Apply operator. */
	result=fftfiltApplyOperator(pinputGreyMap,
		poutputGreyMap,
		freq_resp,
		is_complex);

	/* Deallocate frequency response greymap. */
	riplFree(freq_resp);

	/* Return relevant exit code. */
	if (result) return 2;
	return RIPL_EXECUTEERROR;
}

/* Display help screen. */
const char *fftfiltHelp(void) {
	return " fftfilt <resp-file> <is-complex>\n\n"
		"   Fourier-transforms input image and multiplies the resultant\n"
		"   spectrum with that specified in the text file <resp-file>.\n"
		"   <resp-file> must contain the positive-quadrant frequency\n"
		"   response. <is-complex> specifies whether the frequency response\n"
		"   is real (false) or complex (true). For an m-by-n input image,\n"
		"   the text file must contain (m/2+1)*(n/2+1) (or 2*(m/2+1)*(n/2+1)\n"
		"   if the response is complex for real and imaginary parts)---a\n"
		"   multiplier for each column-wise frequency from 0 to the column\n"
		"   Nyquist frequency for each row-wise frequency from 0 to the row\n"
		"   Nyquist frequency. Can be used to implement arbitrary frequency-\n"
		"   response filters.\n";
}


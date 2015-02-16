/*
 *		riplwave.h
 *		Header file for Ripl Wavelet Transform Toolbox.
 *
 *		Version 1.1, last update: 23 February 1998.
 *
 *		History:
 *			23/2/98:		added some more filter types from Lewis95.
 *			23/2/98:		added some more Daubechies coefficients.
 *			23/2/98:		revised copyright message.
 *			31/1/98:		modified definition of 'riplwtFilter'.
 *			30/1/98:		modified definition of 'riplwtFilter'.
 *			25/1/98:		first implemented.
 *
 *		Several functions based loosely on the specified functions
 *		published in Press92.
 *
 *		Coefficients from Press92, Chui97 and Lewis95.
 *
 *		Parts copyright © 1998, Richard A. Cook.
 */
#ifndef _RIPLWAVE_H_INCLUDED
#define _RIPLWAVE_H_INCLUDED

#include "ripltdfs.h"

/* Wavelet filter function prototype. */
typedef void (*riplwtFiltFunc)(float *data,
	unsigned long n,
	riplTransformType type,
	void *arg);
/* Generic wavelet filter structure. */
typedef struct tagriplwtFilter {
	unsigned ncoeffs;
	float *coeff;
} riplwtFilter;

/* Enumerated data type for specifying wavelet filter type. */
typedef enum tagriplwtFilterType {
	ftInvalid=0,
	ftBattleLemarie24,
	ftCoiflet6,
	ftCoiflet12,
	ftCoiflet18,
	ftDaub4,
	ftDaub6,
	ftDaub8,
	ftDaub10,
	ftDaub12,
	ftDaub14,
	ftDaub16,
	ftDaub18,
	ftDaub20,
	ftHaar2,
	ftAlsoInvalid
} riplwtFilterType;

/* Perform 1D forward or inverse discrete wavelet transform. */
void riplwt1DWT(float *data,
	unsigned long n,
	riplTransformType type,
	riplwtFiltFunc pfiltFunc,
	void *arg);
/* Perform multidimensional forward or inverse discrete wavelet transform. */
void riplwtNDWT(float *data,
	unsigned long *nn,
	unsigned ndim,
	riplTransformType type,
	riplwtFiltFunc pfiltFunc,
	void *arg);
/* Perform 2D forward or inverse discrete wavelet transform. */
void riplwt2DWT(float *data,
	unsigned long rows,
	unsigned long cols,
	riplTransformType type,
	riplwtFiltFunc pfiltFunc,
	void *arg);
/* General-purpose partial wavelet filter function. */
void riplwtFilterFunc(float *data,
	unsigned long len,
	riplTransformType type,
	void *arg);
/* Set up wavelet filter coefficient struct. */
void riplwtSetupFilter(riplwtFilterType coeffs,
	riplwtFilter *pfilt);

#endif

/*
 *		riplfft.h
 *		Header file for Ripl Fast Fourier Transform Toolbox.
 *
 *		Version 1.1, last update: 23 February 1998.
 *
 *		History:
 *			23/2/98:		changed copyright message.
 *			25/1/98:		created 'ripltdfs.h' and modified accordingly.
 *			23/1/98:		created first version of FFT Toolbox from 'miscfft.c'.
 *			21/1/98:		added more FFT stuff.
 *			21/1/98:		moved FFT functions from 'misc.c' to 'miscfft.c'.
 *
 *		Several functions based heavily on the specified functions
 *		published in 'Numerical Recipes in C' (2nd edition),
 *		by Press/Teukolsky/Vetterling/Flannery.
 *
 *		Parts copyright © 1998, Richard A. Cook.
 */
#ifndef _RIPLFFT_H_INCLUDED
#define _RIPLFFT_H_INCLUDED

#include "ripltdfs.h"

/* Convert real float vector into complex float vector/matrix. */
void riplfftCreateCmplxVctF(float *data,
    unsigned long size,
    float *cmplx_vct);
/* Convert vector of grey levels into complex float vector/matrix. */
void riplfftCreateCmplxVctG(riplGrey *data,
    unsigned long size,
    float *cmplx_vct);
/* Calculate (inverse) FT of 1D complex data set. */
void riplfftComplex1DFT(float *data,
    unsigned long nn,
    riplTransformType type);
/* Scale vector of complex data. */
void riplfftComplexScale(float *data,
    unsigned long size,
    unsigned long scale);
/* Calculate (inverse) FT of 1D real data set. */
void riplfftReal1DFT(float *data,
    unsigned long nn,
    riplTransformType type);
/* Scale vector of real data. */
void riplfftRealScale(float *data,
    unsigned long size,
    unsigned long scale);
/* Calculate (inverse) sine transform of 1D real data set. */
void riplfftReal1DST(float *data,
    unsigned long nn);
/* Calculate (inverse) cosine transform of 1D real data set. */
void riplfftReal1DCT1(float *data,
    unsigned long nn);
/* Calculate (inverse) staggered cosine transform of 1D real data set. */
void riplfftReal1DCT2(float *data,
    unsigned long nn,
    riplTransformType type);
/* Calculate (inverse) FT of n-dimensional complex data set. */
void riplfftComplexNDFT(float *data,
    unsigned long *dim,
    unsigned dims,
    riplTransformType type);
/* Calculate (inverse) FT of 2-dimensional complex data set. */
void riplfftComplex2DFT(float *data,
    unsigned long rows,
    unsigned long cols,
    riplTransformType type);
/* Calculate (inverse) FT of 2-dimensional real data set. */
void riplfftReal2DFT(float *data,
    float *nyquist,
    unsigned long rows,
    unsigned long cols,
    riplTransformType type);
/* Convert partial spectrum into full complex spectrum. */
void riplfftGenFullSpectrum(float *data,
    float *nyquist,
    unsigned long rows,
    unsigned long cols,
    float *spec);

#endif

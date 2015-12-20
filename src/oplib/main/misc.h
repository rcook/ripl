/*
 *		misc.h
 *		Header file for miscellaneous image-processing support functions.
 *
 *		Version 1.1, last update: 9 May 1998.
 *
 *		History:
 *			9/5/98:		added miscSubimageF/G function.
 *			10/2/98:		added rescaling functions.
 *			23/1/98:		removed definitions of mathematical constants.
 *			21/1/98:		moved FFT functions to new file 'miscfft.c'.
 *			21/1/98:		added more FFT stuff.
 *			19/1/98:		added FFT functions.
 *			24/12/97:	bug in miscCarve fixed.
 *			16/12/97:	comments updated.
 *			15/12/97:	added miscCarve function.
 *			27/11/97:	first serious version.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#ifndef _MISC_H_INCLUDED
#define _MISC_H_INCLUDED

#include "ripl.h"

/* Types required by miscSubimage. */
typedef enum tagmiscWindowProfile {
    wpInvalid,
    wpCosine,
    wpTriangular
} miscWindowProfile;

/* Subimage function (float version). */
typedef void (*miscSubimageFunctionF)(float *,
    unsigned rows,			/* originally unsigned long */
    unsigned cols,			/* originally unsigned long */
    void *arg);

/* Subimage function (riplGrey version). */
typedef void (*miscSubimageFunctionG)(riplGrey *,
    unsigned rows,			/* originally unsigned long */
    unsigned cols,			/* originally unsigned long */
    void *arg);

/* Enumeration typedef defining window function type. */
typedef enum tagmiscWindowFunction {
    wfInvalid,
    wfRectangular,
    wfHanning,
    wfHamming,
    wfTriangular
} miscWindowFunction;

/* Enumeration typedef identifying type of operator. */
typedef enum tagmiscOperatorType {
    otInvalid,
    otErosion,
    otDilation,
    otOpening,
    otClosing
} miscOperatorType;

/* Enumeration typedef identifying predefined structuring elements. */
typedef enum tagmiscPredefinedStruct {
    psNone,
    psInvalid=psNone,
    psOnes,
    psCross,
    psKite,
    psDiag
} miscPredefinedStruct;

/* Subimage-handling function prototype typedef. */
typedef bool (*miscSubimageFunc)(riplGreyMap *pgreymap,
    unsigned col,
    unsigned row,
    unsigned width,
    unsigned height,
    unsigned classno,
    void *arg);

void miscSetImage(riplGreyMap *pgreyMap, riplGrey fillValue);
bool miscGenerateGaussian(double var,
    unsigned *pkernelSize,
    double **pkernel);
bool miscConvolve1DFP(const riplGrey *pinputData,
    riplGrey *poutputData,
    unsigned sequence_length,
    unsigned step,
    const double *pkernel,
    unsigned kernel_length);
bool miscConvolve2DInt(const riplGrey *pinputData,
    riplGrey *poutputData,
    unsigned image_cols,
    unsigned image_rows,
    const int *pkernel,
    unsigned kernel_cols,
    unsigned kernel_rows,
    unsigned scale,
    bool store_abs);
bool miscConvolve2DFP(const riplGrey *pinputData,
    riplGrey *poutputData,
    unsigned image_cols,
    unsigned image_rows,
    const double *pkernel,
    unsigned kernel_cols,
    unsigned kernel_rows,
    unsigned scale,
    bool store_abs);
void miscGenerateWindowF(miscWindowFunction win_func,
    float *win_data,
    unsigned win_length);
void miscGenerateWindowD(miscWindowFunction win_func,
    double *win_data,
    unsigned win_length);
void miscApplyWindowF(miscWindowFunction win_func,
    float *win_data,
    unsigned win_length);
void miscApplyWindowD(miscWindowFunction win_func,
    double *win_data,
    unsigned win_length);
bool miscHistogramEQ(const riplGrey *pinputData,
    riplGrey *poutputData,
    unsigned pixels);
bool *miscGenerateMorphStruct(miscPredefinedStruct predef,
    unsigned struct_el_cols,
    unsigned struct_el_rows);
bool miscCarve(riplGreyMap *pgreymap,
    riplGrey conf,
    unsigned nclasses,
    const riplGrey *boundary,
    unsigned max_win_width,
    unsigned max_win_height,
    unsigned min_win_width,
    unsigned min_win_height,
    miscSubimageFunc function,
    void *arg);
void miscRescaleF(float *in_vector,
    riplGrey *out_vector,
    unsigned long size);
void miscRescaleD(double *in_vector,
    riplGrey *out_vector,
    unsigned long size);
void miscRescaleG(riplGrey *in_vector,
    riplGrey *out_vector,
    unsigned long size);
void miscRescaleMG(riplMidGrey *in_vector,
    riplGrey *out_vector,
    unsigned long size);
void miscRescaleBG(riplBigGrey *in_vector,
    riplGrey *out_vector,
    unsigned long size);
bool miscSubimageF(const riplGrey *pinput,
    riplGrey *poutput,
    unsigned rows,				/* originally unsigned long */
    unsigned cols,				/* originally unsigned long */
    unsigned w_r,
    unsigned w_c,
    unsigned d_r,
    unsigned d_c,
    miscWindowProfile wp,
    miscSubimageFunctionF func,
    void *arg);
bool miscSubimageG(const riplGrey *pinput,
    riplGrey *poutput,
    unsigned rows,				/* originally unsigned long */
    unsigned cols,				/* originally unsigned long */
    unsigned w_r,
    unsigned w_c,
    unsigned d_r,
    unsigned d_c,
    miscWindowProfile wp,
    miscSubimageFunctionG func,
    void *arg);

#endif

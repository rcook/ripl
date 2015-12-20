/*
 *		conv.h
 *		Header file for convolution of arbitrary 2D integer kernel with image.
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
#ifndef _CONV_H_INCLUDED
#define _CONV_H_INCLUDED

#include "ripl.h"

/* Enumeration typedef identifying type of convolution. */
typedef enum tagconvKernelType {
    ktInvalid,
    ktint,
    ktdouble
} convKernelType;

/* Command-line version. */
int convExecute(unsigned argc,
    const char **argv,
    riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap);
const char *convHelp(void);
/* Internal entrypoint. */
bool convApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    unsigned kernel_columns,
    unsigned kernel_rows,
    convKernelType kernel_type,
    void *kernel);

#endif

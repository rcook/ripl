/*
 *		dllconv.h
 *		Header file for dllconvolution of arbitrary 2D integer kernel with image.
 *		Slightly modified syntax for use with Racdll Tcl/Tk interface.
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
#ifndef _DLLCONV_H_INCLUDED
#define _DLLCONV_H_INCLUDED

#include "ripl.h"

/* Enumeration typedef identifying type of dllconvolution. */
typedef enum tagdllconvKernelType {
	dllktInvalid,
	dllktint,
	dllktdouble
} dllconvKernelType;

/* Command-line version. */
int dllconvExecute(unsigned argc,
	const char **argv,
	riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap);
const char *dllconvHelp(void);
/* Internal entrypoint. */
riplBool dllconvApplyOperator(riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap,
	unsigned scale,
	riplBool store_abs,
	unsigned kernel_columns,
	unsigned kernel_rows,
	dllconvKernelType kernel_type,
	void *kernel);

#endif

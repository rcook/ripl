/*
 *		RIPL---Richard's Image-Processing Library.
 *		Written by Richard Cook.
 *
 *		riplpars.c
 *		Header file for parsing and string matching stuff.
 *
 *		Version 1.1, last update: 20 January 1998.
 *
 *		History:
 *			20/1/98:	first implemented
 *						(moved from riplop to separate file).
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#ifndef _RIPLPARS_H_INCLUDED
#define _RIPLPARS_H_INCLUDED

#include "ripldefs.h"
#include "riplmisc.h"

bool riplArgGet_bool(const char *text,
    bool *value);
bool riplArgGet_riplGrey(const char *text,
    riplGrey *value);
bool riplArgGet_unsigned(const char *text,
    unsigned *value);
bool riplArgGet_int(const char *text,
    int *value);
bool riplArgGet_long(const char *text,
    long *value);
bool riplArgGet_float(const char *text,
    float *value);
bool riplArgGet_double(const char *text,
    double *value);
bool riplMatch(const char *input,
    const char *ref,
    unsigned match_chars);

#endif

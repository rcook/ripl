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
#include "riplmain-defs.h"
#include "riplmisc.h"

RIPL_RIPLMAIN_API bool riplArgGet_bool(
    const char* text,
    bool* value);

RIPL_RIPLMAIN_API bool riplArgGet_riplGrey(
    const char* text,
    riplGrey* value);

RIPL_RIPLMAIN_API bool riplArgGet_unsigned(
    const char* text,
    unsigned* value);

RIPL_RIPLMAIN_API bool riplArgGet_int(
    const char* text,
    int* value);

bool riplArgGet_long(const char *text,
    long *value);

RIPL_RIPLMAIN_API bool riplArgGet_float(
    const char* text,
    float* value);

RIPL_RIPLMAIN_API bool riplArgGet_double(
    const char* text,
    double* value);

RIPL_RIPLMAIN_API bool riplMatch(
    const char*input,
    const char* ref,
    unsigned match_chars);

#endif

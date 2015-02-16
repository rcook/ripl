/*
 *		nop.c
 *		No-op operator
 *
 *		Copyright (C) 1997/8, 2015, Richard A. Cook.
 */

#pragma once

#include "ripl.h"

// Internal entrypoint
riplBool nopApplyOperator(
    riplGreyMap const *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    riplGrey threshold);

// Command-line entrypoint
int nopExecute(
    unsigned argc,
    char const **argv,
    riplGreyMap /*const*/ *pinputGreyMap,
    riplGreyMap *poutputGreyMap);

// Command-line help
char const *nopHelp(void);


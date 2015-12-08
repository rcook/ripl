/*
 *		nop.c
 *		Implementation of no-op operator
 *
 *		Copyright (C) 1997/8, 2015, Richard A. Cook.
 */

#include "nop.h"
#include <string.h>

// Internal entrypoint
bool nopApplyOperator(
    riplGreyMap const *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    riplGrey threshold)
{
    riplGrey const *inP = pinputGreyMap->data;
    riplGrey *outP = poutputGreyMap->data;

    RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap)

    // Just copy straight from input to output
    memcpy(
      poutputGreyMap->data,
      pinputGreyMap->data,
      pinputGreyMap->size * sizeof(riplGrey));

    return true;
}

// Command-line version
int nopExecute(
    unsigned argc,
    char const **argv,
    riplGreyMap /*const*/ *pinputGreyMap,
    riplGreyMap *poutputGreyMap)
{
    riplGrey threshold = 0;

    if (argc > 0)
    {
        riplMessage(
            itError,
            "Incorrect number of parameters!\n"
            "Usage: " RIPL_EXENAME " " RIPL_CMDLINE " nop\n");
        return RIPL_PARAMERROR;
    }

    if (!nopApplyOperator(pinputGreyMap, poutputGreyMap, threshold))
    {
        return RIPL_EXECUTEERROR;
    }

    // Number of command-line arguments consumed
    return 0;
}

// Command-line help
char const *nopHelp(void)
{
    return " nop\n\n"
        "   Do nothing: copy input image to output.\n";
}


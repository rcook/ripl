/*
 *		RIPL---Richard's Image-Processing Library.
 *		Written by Richard Cook.
 *
 *		riplfilt.c
 *		Source file for graphic import/export filters and support functions.
 *
 *		Version 1.1, last update: 20 January 1998.
 *
 *		History:
 *			20/1/98:		version 1.1.
 *			27/11/97:	first implemented.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */

#include "riplfilt.h"

#include "ripldbug.h"
#include "riplpbm.h"
#include "validate.h"
#include "Error.h"
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <memory>

using namespace std;
using namespace ripl;

// Loads specified image converting it to greyscale if required
riplGreyMap riplLoadImage(const char* fileName)
{
    RIPL_VALIDATE_ARG_NAME(fileName, "fileName");

    // Read format of image file
    auto graphicFormat = riplReadGraphicFormat(fileName);

    // Load file using appropriate filter and return pointer to it
    switch (graphicFormat)
    {
    case gfPBMASCII:
    case gfPGMASCII:
    case gfPPMASCII:
    case gfPBMBinary:
    case gfPGMBinary:
    case gfPPMBinary:
        return riplPBMLoadFile(fileName, graphicFormat);
    default:
        RIPL_VALIDATE_FAIL(error::InvalidOperation);
    }
}

// Saves specified image under specified file name in specified format
void riplSaveImage(
    const char* fileName,
    riplGraphicFormat graphicFormat,
    const riplGreyMap& greyMap)
{
    RIPL_VALIDATE_ARG_NAME(fileName, "fileName");

    // Save file using appropriate filter
    switch (graphicFormat)
    {
    case gfPBMASCII:
    case gfPGMASCII:
    case gfPPMASCII:
    case gfPBMBinary:
    case gfPGMBinary:
    case gfPPMBinary:
        riplPBMSaveFile(fileName, graphicFormat, greyMap);
        break;
    default:
        RIPL_VALIDATE_ARG_NAME_FAIL("graphicFormat");
    }
}

/* Determine the graphic-file format of the specified file. */
riplGraphicFormat riplReadGraphicFormat(const char* fileName)
{
    RIPL_VALIDATE_ARG_NAME(fileName, "fileName");

    unique_ptr<FILE, function<int(FILE*)>> file(fopen(fileName, "rb"), fclose);
    RIPL_VALIDATE_NEW(file, error::IOError);

    if (static_cast<char>(fgetc(file.get())) != 'P')
    {
        return gfInvalid;
    }

    // It's a P, therefore it's likely to be PBM/PGM/PPM
    switch (static_cast<char>(fgetc(file.get())))
    {
    case '1': return gfPBMASCII;
    case '2': return gfPGMASCII;
    case '3': return gfPPMASCII;
    case '4': return gfPBMBinary;
    case '5': return gfPGMBinary;
    case '6': return gfPPMBinary;
    default: return gfInvalid;
    }
}

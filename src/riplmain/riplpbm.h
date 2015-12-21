#pragma once

#include "riplgrey.h"

/** Loads a Netpbm-format image from specified stream */
riplGreyMap netpbmLoad(std::istream& stream, ImageFormat format);

/** Loads a Netpbm-format image from specified file */
riplGreyMap netpbmLoad(const char* fileName, ImageFormat format);

/** Saves specified image to specified file in specified Netpbm format */
void netpbmSave(
    const char* fileName,
    ImageFormat format,
    const riplGreyMap& image);

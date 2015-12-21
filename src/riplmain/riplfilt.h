#pragma once

#include "riplgrey.h"

/** Loads specified image from the specified stream converting it to greyscale if required */
riplGreyMap riplLoadImage(std::istream& stream);

/** Loads specified image from the specified file converting it to greyscale if required */
riplGreyMap riplLoadImage(const char* fileName);

/** Saves specified image under specified file name in specified format */
void riplSaveImage(
    const char* fileName,
    ImageFormat format,
    const riplGreyMap& image);

/** Determines the graphic format from the specified stream */
ImageFormat riplReadGraphicFormat(std::istream& stream);

/** Determines the graphic format of the specified file */
ImageFormat riplReadGraphicFormat(const char* fileName);

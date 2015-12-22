#pragma once

#include "riplgrey.h"

/** Loads image from specified stream converting it to greyscale if required */
riplGreyMap riplLoadImage(std::istream& stream);

/** Loads image from specified buffer converting it to greyscale if required */
riplGreyMap riplLoadImage(const char* buffer, size_t bufferSize);

/** Loads image from specified file converting it to greyscale if required */
riplGreyMap riplLoadImage(const char* fileName);

/** Saves specified image with specified file name in specified format */
void riplSaveImage(
    const char* fileName,
    ImageFormat format,
    const riplGreyMap& image);

/** Determines the image format from the specified stream */
ImageFormat riplReadGraphicFormat(std::istream& stream);

/** Determines the image format from the specified buffer */
ImageFormat riplReadGraphicFormat(const char* buffer, size_t bufferSize);

/** Determines the image format of the specified file */
ImageFormat riplReadGraphicFormat(const char* fileName);

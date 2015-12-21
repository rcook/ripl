#include "riplfilt.h"

#include "riplpbm.h"
#include <fstream>

using namespace std;
using namespace ripl;

riplGreyMap riplLoadImage(istream& stream)
{
    // Read format of image file
    auto format = riplReadGraphicFormat(stream);

    // Load file using appropriate filter and return pointer to it
    switch (format)
    {
    case ImageFormat::PgmAscii:
    case ImageFormat::PgmBinary:
        return netpbmLoad(stream, format);
    default:
        RIPL_REQUIRE_FAIL(error::InvalidOperation);
    }
}

riplGreyMap riplLoadImage(const char* fileName)
{
    RIPL_VALIDATE_ARG_NAME(fileName, "fileName");

    ifstream stream(fileName, ios::in | ios::binary);
    RIPL_REQUIRE(stream, error::IOError);

    return riplLoadImage(stream);
}

void riplSaveImage(
    const char* fileName,
    ImageFormat format,
    const riplGreyMap& greyMap)
{
    RIPL_VALIDATE_ARG_NAME(fileName, "fileName");
    RIPL_VALIDATE_ARG_NAME(format == ImageFormat::PgmAscii || format == ImageFormat::PgmBinary, "format");

    netpbmSave(fileName, format, greyMap);
}

ImageFormat riplReadGraphicFormat(istream& stream)
{
    if (static_cast<char>(stream.get()) != 'P')
    {
        return ImageFormat::Invalid;
    }

    // It's a P, therefore it's likely to be PBM/PGM/PPM
    switch (static_cast<char>(stream.get()))
    {
    case '1': return ImageFormat::PbmAscii;
    case '2': return ImageFormat::PgmAscii;
    case '3': return ImageFormat::PpmAscii;
    case '4': return ImageFormat::PbmBinary;
    case '5': return ImageFormat::PgmBinary;
    case '6': return ImageFormat::PpmBinary;
    default: return ImageFormat::Invalid;
    }
}

ImageFormat riplReadGraphicFormat(const char* fileName)
{
    RIPL_VALIDATE_ARG_NAME(fileName, "fileName");

    ifstream stream(fileName, ios::in | ios::binary);
    RIPL_REQUIRE(stream, error::IOError);

    return riplReadGraphicFormat(stream);
}

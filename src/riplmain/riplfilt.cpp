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
    case gfPGMASCII:
    case gfPGMBinary:
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
    riplGraphicFormat format,
    const riplGreyMap& greyMap)
{
    RIPL_VALIDATE_ARG_NAME(fileName, "fileName");
    RIPL_VALIDATE_ARG_NAME(format == gfPGMASCII || format == gfPGMBinary, "format");

    netpbmSave(fileName, format, greyMap);
}

riplGraphicFormat riplReadGraphicFormat(istream& stream)
{
    if (static_cast<char>(stream.get()) != 'P')
    {
        return gfInvalid;
    }

    // It's a P, therefore it's likely to be PBM/PGM/PPM
    switch (static_cast<char>(stream.get()))
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

riplGraphicFormat riplReadGraphicFormat(const char* fileName)
{
    RIPL_VALIDATE_ARG_NAME(fileName, "fileName");

    ifstream stream(fileName, ios::in | ios::binary);
    RIPL_REQUIRE(stream, error::IOError);

    return riplReadGraphicFormat(stream);
}

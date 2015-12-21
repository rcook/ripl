#include "riplpbm.h"

#include "utillib/ScopedHandle.h"
#include <fstream>
#include <functional>
#include <memory>

using namespace std;
using namespace ripl;

struct Header
{
    Header(image_dimension_t width, image_dimension_t height, int maximumLevel)
        : m_width(width)
        , m_height(height)
        , m_maximumLevel(maximumLevel)
    {
    }

    const image_dimension_t m_width;
    const image_dimension_t m_height;
    const int m_maximumLevel;
};

static Header readHeader(istream& stream)
{
    image_dimension_t width;
    stream >> width;
    RIPL_REQUIRE(width > 0 && width < RIPL_MAX_COLS, error::ImageTooBig);

    image_dimension_t height;
    stream >> height;
    RIPL_REQUIRE(height > 0 && height < RIPL_MAX_ROWS, error::ImageTooBig);

    int maximumLevel;
    stream >> maximumLevel;
    RIPL_REQUIRE(maximumLevel == RIPL_MAX_GREY, error::InvalidFileFormat);

    int c = stream.get();
    RIPL_REQUIRE(c == '\n' || c == '\r' && stream.get() == '\n', error::InvalidFileFormat);

    return Header(width, height, maximumLevel);
}

// P2 format
static void readPgmAscii(istream& stream, vector<riplGrey>& data)
{
    for (size_t i = 0; i < data.size(); ++i)
    {
        int value;
        stream >> value;
        data[i] = value;
    }
}

// P5 format
static void readPgmBinary(istream& stream, vector<riplGrey>& data)
{
    stream.read(reinterpret_cast<char*>(data.data()), data.size() * sizeof(riplGrey));
}

riplGreyMap netpbmLoad(istream& stream, ImageFormat format)
{
    RIPL_VALIDATE_ARG_NAME(format == ImageFormat::PgmAscii || format == ImageFormat::PgmBinary, "format");

    auto header = readHeader(stream);

    vector<riplGrey> data(header.m_width * header.m_height);
    switch (format)
    {
    case ImageFormat::PgmAscii: // P2
        readPgmAscii(stream, data);
        break;

    case ImageFormat::PgmBinary: // P5
        readPgmBinary(stream, data);
        break;

    default:
        RIPL_VALIDATE_ARG_NAME_FAIL("format");
    }

    RIPL_REQUIRE(!stream.eof(), error::InvalidFileFormat);

    return riplGreyMap(header.m_width, header.m_height, move(data));
}

riplGreyMap netpbmLoad(const char* fileName, ImageFormat format)
{
    RIPL_VALIDATE_ARG_NAME(fileName, "fileName");
    RIPL_VALIDATE_ARG_NAME(format == ImageFormat::PgmAscii || format == ImageFormat::PgmBinary, "format");

    ifstream stream(fileName, ios::in | ios::binary);
    RIPL_REQUIRE(stream, error::IOError);

    if (format == ImageFormat::PgmBinary)
    {
        RIPL_REQUIRE(stream.peek() == EOF, error::InvalidFileFormat);
    }

    return netpbmLoad(stream, format);
}

void netpbmSave(const char* fileName, ImageFormat format, const riplGreyMap& image)
{
    RIPL_VALIDATE_ARG_NAME(fileName, "fileName");
    RIPL_VALIDATE_ARG_NAME(format == ImageFormat::PgmBinary, "format");

    using FileHandle = ScopedHandle<FILE*, nullptr, decltype(fclose)*>;

    FileHandle file(fopen(fileName, "wb"), fclose);
    RIPL_REQUIRE(file, error::IOError);

    fprintf(
        file.get(),
        "P5\n"
        "# %u-level PGM\n"
        "# Generated by RIPL " __DATE__ "\n"
        "%u %u\n%u\n",
        RIPL_GREY_LEVELS,
        image.width(),
        image.height(),
        RIPL_MAX_GREY);
    fwrite(image.data(), sizeof(riplGrey), image.size(), file.get());
}

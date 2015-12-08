/*
 *		RIPL---Richard's Image-Processing Library.
 *		Written by Richard Cook.
 *
 *		riplpbm.c
 *		Source file for Portable Bitmap (PBM/PGM/PPM) import filter functions.
 *
 *		Version 1.1, last update: 10 February 1998.
 *
 *		History:
 *			10/2/98:		added message if image too big.
 *			20/1/98:		version 1.1.
 *			19/1/98:		modified included files list (added 'riplallc.h').
 *			16/12/97:	amendment to bug fix.
 *			15/12/97:	bug fix (newlines in binary formats).
 *			27/11/97:	last substantial update.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */

#include "riplpbm.h"

#include "riplallc.h"
#include "ripldbug.h"
#include "validate.h"
#include <cctype>
#include <cmath>
#include <cstdio>
#include <functional>
#include <memory>

using namespace std;
using namespace ripl;

/* Typedef and constants for  local use only. */
#define BUFFER_SIZE				256
#define TOKEN_SIZE				64

class PbmFileInfo
{
public:
    PbmFileInfo() = delete;
    PbmFileInfo(const PbmFileInfo&) = delete;
    PbmFileInfo& operator=(const PbmFileInfo&) = delete;

public:
    PbmFileInfo(const char* fileName);
    ~PbmFileInfo();

    void skipSpace();
    const char* readToken();
    void findChar(char ch);
    void skipLine();
    const char* getToken();
    void skipChars(unsigned nchars);
    void flushFile();
    void read(void* buffer, size_t size, size_t count);

public:
    FILE* m_file;
    FILE* m_fileHandle;
    riplGrey m_buffer[BUFFER_SIZE];
    char m_token[TOKEN_SIZE + 1];
    size_t m_items;
    size_t m_pos;
};

// Reads ASCII-encoded portable bitmap
static void readPbmAscii(PbmFileInfo& pbmFile, riplGreyMap& greyMap)
{
    auto p = greyMap.data();
    for (decltype(greyMap.size()) i = 0; i < greyMap.size(); ++i)
    {
        *p++ = (1 - atof(pbmFile.getToken())) * RIPL_MAX_GREY;
    }
}

// Reads ASCII-encoded portable grey map
static void readPgmAscii(PbmFileInfo& pbmFile, riplGreyMap& greyMap)
{
    unsigned maxIntensity = atoi(pbmFile.getToken());
    if (maxIntensity == RIPL_MAX_GREY)
    {
        auto p = greyMap.data();
        for (decltype(greyMap.size()) i = 0; i < greyMap.size(); ++i)
        {
            *p++ = static_cast<riplGrey>(atoi(pbmFile.getToken()));
        }
    }
    else
    {
        auto p = greyMap.data();
        for (decltype(greyMap.size()) i = 0; i < greyMap.size(); ++i)
        {
            *p++ = static_cast<riplGrey>((atof(pbmFile.getToken()) / maxIntensity * RIPL_MAX_GREY));
        }
    }
}

// Reads ASCII-encoded portable pixel map
static void readPpmAscii(PbmFileInfo& pbmFile, riplGreyMap& greyMap)
{
    unsigned maxIntensity = atoi(pbmFile.getToken());
    if (maxIntensity == RIPL_MAX_GREY)
    {
        auto p = greyMap.data();
        for (decltype(greyMap.size()) i = 0; i < greyMap.size(); ++i)
        {
            *p++ = static_cast<riplGrey>(riplRound(
                RIPL_RED_WEIGHT * atoi(pbmFile.getToken())
                + RIPL_GREEN_WEIGHT * atoi(pbmFile.getToken())
                + RIPL_BLUE_WEIGHT * atoi(pbmFile.getToken())));
        }
    }
    else
    {
        auto p = greyMap.data();
        for (decltype(greyMap.size()) i = 0; i < greyMap.size(); ++i)
        {
            *p++ = static_cast<riplGrey>(riplRound((
                RIPL_RED_WEIGHT * atof(pbmFile.getToken())
                + RIPL_GREEN_WEIGHT * atof(pbmFile.getToken())
                + RIPL_BLUE_WEIGHT * atof(pbmFile.getToken()))
                / maxIntensity * RIPL_MAX_GREY));
        }
    }
}

// Reads binary-encoded portable bitmap
static void readPbmBinary(PbmFileInfo& pbmFile, riplGreyMap& greyMap)
{
    pbmFile.skipChars(1);
    pbmFile.flushFile();

    unsigned dataSize = greyMap.width() >> 3;
    if ((dataSize % 8) != 0)
    {
        ++dataSize;
    }

    unique_ptr<riplGrey[]> buffer(new riplGrey[dataSize]);
    auto p = greyMap.data();
    for (decltype(greyMap.height()) row = 0; row < greyMap.height(); ++row)
    {
        pbmFile.read(buffer.get(), sizeof(riplGrey), dataSize);
        auto bufferPtr = buffer.get();
        riplGrey value;
        for (decltype(greyMap.width()) col = 0; col < greyMap.width(); ++col, value <<= 1)
        {
            if (col % 8 == 0)
            {
                value = *bufferPtr++;
            }

            *p++ = static_cast<riplGrey>((1 - (value & 0x80) > 0) * RIPL_MAX_GREY);
        }
    }
}

// Reads binary-encoded portable grey map
static void readPgmBinary(PbmFileInfo& pbmFile, riplGreyMap& greyMap)
{
    unsigned maxIntensity = atoi(pbmFile.getToken());
    RIPL_VALIDATE_NEW(maxIntensity == RIPL_MAX_GREY, error::InvalidOperation);
    pbmFile.skipChars(1);
    pbmFile.flushFile();
    pbmFile.read(greyMap.data(), sizeof(riplGrey), greyMap.size());
}

// Reads binary-encoded portable pixel map
static void readPpmBinary(PbmFileInfo& pbmFile, riplGreyMap& greyMap)
{
    unsigned maxIntensity = atoi(pbmFile.getToken());
    RIPL_VALIDATE_NEW(maxIntensity == RIPL_MAX_GREY, error::InvalidOperation);
    pbmFile.skipChars(1);
    pbmFile.flushFile();
    
    unsigned dataSize = greyMap.width() * 3;

    unique_ptr<riplGrey[]> buffer(new riplGrey[dataSize]);
    auto p = greyMap.data();
    for (decltype(greyMap.height()) row = 0; row < greyMap.height(); ++row)
    {
        pbmFile.read(buffer.get(), sizeof(riplGrey), dataSize);
        auto bufferPtr = buffer.get();
        for (decltype(greyMap.width()) col = 0; col < greyMap.width(); ++col, bufferPtr += 3)
        {
            *p++ = static_cast<riplGrey>(riplRound(
                RIPL_RED_WEIGHT * bufferPtr[0]
                + RIPL_GREEN_WEIGHT * bufferPtr[1]
                + RIPL_BLUE_WEIGHT * bufferPtr[2]));
        }
    }
}

// Loads a PBM-format file and returns a new grey map object
riplGreyMap riplPBMLoadFile(const char* fileName, riplGraphicFormat graphicFormat)
{
    RIPL_VALIDATE_ARG_NAME(fileName, "fileName");

    // Read in and discard the P token
    PbmFileInfo pbmFile(fileName);
    pbmFile.readToken();

    // Read in the number of columns and rows
    auto width = atoi(pbmFile.getToken());
    auto height = atoi(pbmFile.getToken());
    RIPL_VALIDATE_NEW(
        width <= RIPL_MAX_COLS && height <= RIPL_MAX_ROWS,
        error::ImageTooBig);

    // Create empty grey map
    riplGreyMap greyMap(width, height);

    // Read it in
    switch (graphicFormat)
    {
    case gfPBMASCII: readPbmAscii(pbmFile, greyMap); break;
    case gfPGMASCII: readPgmAscii(pbmFile, greyMap); break;
    case gfPPMASCII: readPpmAscii(pbmFile, greyMap); break;
    case gfPBMBinary: readPbmBinary(pbmFile, greyMap); break;
    case gfPGMBinary: readPgmBinary(pbmFile, greyMap); break;
    case gfPPMBinary: readPpmBinary(pbmFile, greyMap); break;
    default:
        RIPL_VALIDATE_ARG_NAME_FAIL("graphicFormat");
    }

    return greyMap;
}

// Saves specified grey map with specified file name and in specified subformat
void riplPBMSaveFile(const char* fileName, riplGraphicFormat graphicFormat, const riplGreyMap& greyMap)
{
    RIPL_VALIDATE_ARG_NAME(fileName, "fileName");
    RIPL_VALIDATE_ARG_NAME(graphicFormat == gfPGMBinary, "graphicFormat");

    unique_ptr<FILE, function<int(FILE*)>> file(fopen(fileName, "wb"), fclose);
    RIPL_VALIDATE_NEW(file, error::IOError);

    fprintf(
        file.get(),
        "P5\n"
        "# %u-Level Portable Greymap.\n"
        "# PGM Export Filter by Richard Cook " __DATE__ "\n"
        "%u %u\n%u\n",
        RIPL_GREY_LEVELS,
        greyMap.width(), greyMap.height(),
        RIPL_MAX_GREY);
    fwrite(greyMap.data(), sizeof(riplGrey), greyMap.size(), file.get());
}

PbmFileInfo::PbmFileInfo(const char* fileName)
    : m_file(fopen(fileName, "rb"))
    , m_fileHandle(nullptr)
    , m_items(0)
    , m_pos(0)
{
    RIPL_VALIDATE_NEW(m_file, error::FileNotFound);
}

PbmFileInfo::~PbmFileInfo()
{
    if (m_file)
    {
        fclose(m_file);
        m_file = nullptr;
    }

    if (m_fileHandle)
    {
        fclose(m_fileHandle);
        m_fileHandle = nullptr;
    }
}

void PbmFileInfo::skipSpace()
{
    do
    {
        while (m_pos < m_items && isspace(m_buffer[m_pos]))
        {
            ++m_pos;
        }

        if (m_pos == m_items)
        {
            RIPL_VALIDATE_NEW(!feof(m_fileHandle), error::EndOfFile);
            m_items = fread(m_buffer, sizeof(char), BUFFER_SIZE, m_fileHandle);
            RIPL_VALIDATE_NEW(!ferror(m_fileHandle), error::IOError);
            m_pos = 0;
        }
    }
    while (isspace(m_buffer[m_pos]));
}

const char* PbmFileInfo::readToken()
{
    size_t start_pos;
    unsigned token_len;
    char *ptr;

    if (m_file)
    {
        m_fileHandle = m_file;
        m_file = nullptr;
        m_items = fread(m_buffer, sizeof(char), BUFFER_SIZE, m_fileHandle);
        RIPL_VALIDATE_NEW(!ferror(m_fileHandle), error::IOError);
        m_pos = 0;
    }
    else
    {
        RIPL_VALIDATE_NEW(m_fileHandle, error::InvalidOperation);
    }

    /* Find beginning of token. */
    skipSpace();
    start_pos = m_pos;
    token_len = 0;

    /* Find end of token. */
    do
    {
        while (m_pos < m_items && !isspace(m_buffer[m_pos]))
        {
            ++m_pos;
        }

        if (m_pos == m_items)
        {
            RIPL_VALIDATE_NEW(!feof(m_fileHandle), error::EndOfFile);
            ptr = m_token + token_len;
            token_len += m_pos - start_pos;
            RIPL_VALIDATE_NEW(token_len <= TOKEN_SIZE, error::InvalidOperation);
            while (start_pos < m_pos)
            {
                *ptr++ = (char) m_buffer[start_pos++];
            }
            m_items = fread(m_buffer,
                sizeof(char), BUFFER_SIZE, m_fileHandle);
            RIPL_VALIDATE_NEW(!ferror(m_fileHandle), error::IOError);
            start_pos = m_pos = 0;
        }
        else
        {
            ptr = m_token + token_len;
            token_len += m_pos - start_pos;
            RIPL_VALIDATE_NEW(token_len <= TOKEN_SIZE, error::InvalidOperation);
            while (start_pos < m_pos)
            {
                *ptr++ = (char) m_buffer[start_pos++];
            }
            break;
        }
    }
    while (!isspace(m_buffer[m_pos]));

    m_token[token_len] = '\0';
    return m_token;
}

void PbmFileInfo::findChar(char ch)
{
    do
    {
        while (m_pos < m_items && m_buffer[m_pos] != ch)
        {
            ++m_pos;
        }

        if (m_pos == m_items)
        {
            RIPL_VALIDATE_NEW(!feof(m_fileHandle), error::EndOfFile);
            m_items = fread(m_buffer, sizeof(char), BUFFER_SIZE, m_fileHandle);
            RIPL_VALIDATE_NEW(!ferror(m_fileHandle), error::IOError);
            m_pos = 0;
        }
    }
    while (m_buffer[m_pos] != ch);
}

void PbmFileInfo::skipLine()
{
    findChar('\n');
    skipSpace();
}

const char* PbmFileInfo::getToken()
{
    const char* token = readToken();
    while (token[0] == '#')
    {
        skipLine();
        token = readToken();
    }

    return token;
}

void PbmFileInfo::skipChars(unsigned nchars)
{
    unsigned skippable;

    do
    {
        skippable = m_items - m_pos;
        if (skippable > nchars)
        {
            m_pos += nchars;
            break;
        }

        nchars -= skippable;
        RIPL_VALIDATE_NEW(!feof(m_fileHandle), error::EndOfFile);
        m_items = fread(m_buffer, sizeof(char), BUFFER_SIZE, m_fileHandle);
        RIPL_VALIDATE_NEW(!ferror(m_fileHandle), error::IOError);
        m_pos = 0;
    }
    while (nchars);
}

void PbmFileInfo::flushFile()
{
    RIPL_VALIDATE_NEW(!fseek(m_fileHandle, m_pos - m_items, SEEK_CUR), error::IOError);

    m_file = m_fileHandle;
    m_fileHandle = nullptr;
    m_items = 0;
    m_pos = 0;
}

void PbmFileInfo::read(void* buffer, size_t size, size_t count)
{
    auto readCount = fread(
        buffer,
        size,
        count,
        m_file);
    RIPL_VALIDATE_NEW(readCount == count, error::EndOfFile);
}

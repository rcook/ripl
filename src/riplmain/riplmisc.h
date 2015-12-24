#pragma once

#include "config.h"
#include "ripldefs.h"
#include "riplmain-defs.h"
#include <string>
#include <vector>

typedef unsigned char riplGrey;
typedef unsigned char riplByte;
typedef unsigned short riplMidGrey;
typedef unsigned long riplBigGrey;
typedef signed short riplMidGreyS;
typedef signed long riplBigGreyS;

#define RIPL_MAX_COLS			4096
#define RIPL_MAX_ROWS			4096
#define RIPL_RED_WEIGHT			0.3
#define RIPL_GREEN_WEIGHT		0.59
#define RIPL_BLUE_WEIGHT		0.11
#define RIPL_GREY_LEVELS		256
/* It doesn't make sense to define the following any other way. */
#define RIPL_MAX_GREY			(RIPL_GREY_LEVELS-1)
/* Return codes. */
#define RIPL_SUCCESS        2
#define RIPL_FIRSTERRORCODE	-1
#define RIPL_PARAMERROR			(RIPL_FIRSTERRORCODE)
#define RIPL_EXECUTEERROR		(RIPL_FIRSTERRORCODE-1)
#define RIPL_USERERROR			(RIPL_FIRSTERRORCODE-2)

enum class ImageFormat
{
    Invalid,
    PbmAscii,
    PbmBinary,
    PgmAscii,
    PgmBinary,
    PpmAscii,
    PpmBinary,
    PcxAny,
    PcxBasic,
    PcxExtended
};

/** Reads command-line arguments from the specified response file */
RIPL_RIPLMAIN_API std::vector<std::string> riplParseResponseFile(const std::string& fileName);

RIPL_RIPLMAIN_API bool riplFileExists(const char* fileName);

RIPL_RIPLMAIN_API int riplRound(double val);

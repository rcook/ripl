#pragma once

#include "riplmain-defs.h"
#include <string>

namespace ripl { namespace error
{
    class RIPL_RIPLMAIN_API Error
    {
    public:
        Error() = delete;
        Error(const Error&) = delete;
        Error& operator=(const Error&) = delete;

    public:
        ~Error() = default;

        Error(const char* message);

        const char* message() const;

    private:
        const char* m_message;
    };

    RIPL_RIPLMAIN_API extern Error OutOfMemory;
    RIPL_RIPLMAIN_API extern Error InvalidOperation;
    RIPL_RIPLMAIN_API extern Error FileNotFound;
    RIPL_RIPLMAIN_API extern Error EndOfFile;
    RIPL_RIPLMAIN_API extern Error IOError;
    RIPL_RIPLMAIN_API extern Error ImageTooBig;
    RIPL_RIPLMAIN_API extern Error InvalidWidth;
    RIPL_RIPLMAIN_API extern Error InvalidHeight;
    RIPL_RIPLMAIN_API extern Error InvalidSize;
    RIPL_RIPLMAIN_API extern Error InvalidFileFormat;
}} // namespace ripl::error

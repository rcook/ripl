#pragma once

#include "common.h"

#ifdef BUILD_RIPLMAIN
#   define RIPL_RIPLMAIN_API DLLEXPORT
#else
#   define RIPL_RIPLMAIN_API DLLIMPORT
#endif

namespace ripl
{
    using image_dimension_t = unsigned;
    using image_size_t = unsigned long;
    using grey_t = unsigned char;
} // namespace ripl

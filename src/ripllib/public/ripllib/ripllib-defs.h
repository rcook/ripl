#pragma once

#include "common.h"

#ifdef BUILD_RIPLLIB
#   define RIPL_RIPLLIB_API DLLEXPORT
#else
#   define RIPL_RIPLLIB_API DLLIMPORT
#endif

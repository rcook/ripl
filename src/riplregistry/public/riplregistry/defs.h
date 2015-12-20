#pragma once

#include "common.h"

#ifdef BUILD_RIPLREGISTRY
#   define RIPLREGISTRY_API DLLEXPORT
#else
#   define RIPLREGISTRY_API DLLIMPORT
#endif

#pragma once

#include "config.h"

#ifdef _MSC_VER
#   define DLLEXPORT __declspec(dllexport)
#   define DLLIMPORT __declspec(dllimport)
#else
#   define DLLEXPORT __attribute__((visibility("default")))
#   define DLLIMPORT
#endif

#ifdef BUILD_INTERFACELIB
#   define INTERFACELIB_API DLLEXPORT
#else
#   define INTERFACELIB_API DLLIMPORT
#endif


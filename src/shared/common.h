#pragma once

#include "config.h"
#include <cctype>

#ifdef BUILD_WINDOWS
#   define DLLEXPORT __declspec(dllexport)
#   define DLLIMPORT __declspec(dllimport)
#else
#   define DLLEXPORT __attribute__((visibility("default")))
#   define DLLIMPORT
#endif

#ifndef HAVE_STRDUP
inline char* strdup(const char* s)
{
    char* result = calloc(strlen(s) + 1, sizeof(char));
    return result ? strcpy(new_s, s) : nullptr;
}
#endif

#ifndef HAVE_STRLWR
inline char* strlwr(char* s)
{
    for (char* p = s; *p; ++p)
    {
        if (isupper(*p))
        {
            *p = tolower(*p);
        }
    }

    return s;
}
#endif

#ifndef HAVE_STRUPR
inline char* strupr(char* s)
{
    for (char* p = s; *p; ++p)
    {
        if (islower(*p))
        {
            *p = toupper(*p);
        }
    }

    return s;
}
#endif

#ifndef HAVE_MAKE_UNIQUE
#include <memory>

namespace std
{
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}
#endif

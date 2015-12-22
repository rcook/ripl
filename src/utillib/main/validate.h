#pragma once

#define UTILLIB_REQUIRE(expr) \
    do \
    { \
        if (!(expr)) \
        { \
            throw std::runtime_error(#expr); \
        } \
    } \
    while (false)

#define UTILLIB_VALIDATE_ARG_NAME(expr, name) \
    do \
    { \
        if (!(expr)) \
        { \
            throw std::invalid_argument(name); \
        } \
    } \
    while (false)


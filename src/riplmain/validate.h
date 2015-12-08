#pragma once

#include "Error.h"
#include <stdexcept>

#define RIPL_VALIDATE_NEW(expr, errorObj) \
    do \
    { \
        if (!(expr)) \
        { \
            throw std::runtime_error(errorObj.message()); \
        } \
    } \
    while (false)

#define RIPL_VALIDATE_FAIL(errorObj) \
    throw std::runtime_error(errorObj.message())

#define RIPL_VALIDATE_ARG(expr, errorObj) \
    do \
    { \
        if (!(expr)) \
        { \
            throw std::invalid_argument(errorObj.message()); \
        } \
    } \
    while (false)

#define RIPL_VALIDATE_ARG_FAIL(errorObj) \
    throw std::invalid_argument(errorObj.message())

#define RIPL_VALIDATE_ARG_NAME(expr, name) \
    do \
    { \
        if (!(expr)) \
        { \
            throw std::invalid_argument(name); \
        } \
    } \
    while (false)

#define RIPL_VALIDATE_ARG_NAME_FAIL(name) \
    throw std::invalid_argument(name)

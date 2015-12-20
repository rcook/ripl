#pragma once

#include "riplgrey.h"
#include <functional>

/**
    Operator execute function

    Function is passed any remaining command-line arguments and pointers
    to the input and output images.

    Returns one of:
    RIPL_PARAMERROR     if a parse error occurs
    RIPL_EXECUTEERROR   if an execution error occurs
    RIPL_USERERROR      if the user supplies invalid values etc.
*/
using ExecuteFunc = std::function<int(unsigned, const char**, riplGreyMap*, riplGreyMap*)>;

// Provides helper string to be displayed to user
using HelpFunc = std::function<const char*()>;

class Registrar
{
public:
    virtual ~Registrar() = default;

    virtual void registerOp(
        const char* name,
        const char* description,
        ExecuteFunc executeFunc,
        HelpFunc helpFunc) = 0;
};

void oplib_registerOps(Registrar& registrar);

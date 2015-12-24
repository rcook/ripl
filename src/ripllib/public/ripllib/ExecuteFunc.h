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

#pragma once

#include "Op.h"
#include <functional>
#include <unordered_map>

const char* riplGetOperatorSummary(const std::unordered_map<std::string, Op>& ops);

bool riplOperatorHelp(
    const std::unordered_map<std::string, Op>& ops,
    const char* name);

unsigned riplOperatorExecute(
    const std::unordered_map<std::string, Op>& ops,
    unsigned argc,
    const char** argv,
    riplGreyMap* input,
    riplGreyMap* output);

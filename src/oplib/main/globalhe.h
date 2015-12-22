#pragma once

#include "ripl.h"
#include <vector>

namespace ripl { namespace oplib {

    /** Returns input image unchanged */
    void globalHE(riplGreyMap& output, const riplGreyMap& input);

    /** Command-line entry point **/
    int globalHEExecute(riplGreyMap& output, const std::vector<std::string>& args, const riplGreyMap& input);

    /** Provide help for operator */
    const char* globalHEHelp();

}} // namespace ripl::oplib


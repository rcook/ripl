#pragma once

#include "ripl.h"
#include <vector>

namespace ripl { namespace oplib {

    /** Returns input image unchanged */
    void nop(riplGreyMap& output, const riplGreyMap& input);

    /** Command-line entry point **/
    int nopExecute(riplGreyMap& output, const std::vector<std::string>& args, const riplGreyMap& input);

    /** Provide help for operator */
    const char* nopHelp();

}} // namespace ripl::oplib

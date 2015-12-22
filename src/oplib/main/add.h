#pragma once

#include "riplgrey.h"

namespace ripl { namespace oplib {

    /** Adds two images together with specified weight applied to second image */
    void add(riplGreyMap& output, const riplGreyMap& input0, const riplGreyMap& input1, float weight);

}} // namespace ripl::oplib

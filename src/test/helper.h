#pragma once

#include "riplgrey.h"
#include <vector>

namespace ripl { namespace test { namespace helper {
    bool compareImageData(
        unsigned width,
        unsigned height,
        const std::vector<riplGrey>& data,
        const riplGreyMap& image);
}}} // namespace ripl::test::helper

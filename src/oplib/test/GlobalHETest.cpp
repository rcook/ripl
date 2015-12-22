#include "catch.hpp"
#include "globalhe.h"
#include "resources.h"
#include "riplfilt.h"
#include <cstring>

using namespace resources;
using namespace ripl::oplib;

TEST_CASE("globalHE", "globalHE")
{
    SECTION("basics")
    {
        auto input = riplLoadImage(lena_pgm.data(), lena_pgm.size());
        riplGreyMap output(input.width(), input.height());
        globalHE(output, input);
        CHECK(lena_globalhe_dat.size() == output.size());
        CHECK(memcmp(lena_globalhe_dat.data(), output.data(), lena_globalhe_dat.size()) == 0);
    }
}


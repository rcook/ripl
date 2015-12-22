#include "catch.hpp"
#include "nop.h"
#include "resources.h"
#include "riplfilt.h"

using namespace resources;
using namespace ripl::oplib;

TEST_CASE("nop", "nop")
{
    SECTION("basics")
    {
        auto input = riplLoadImage(lena_pgm.data(), lena_pgm.size());
        riplGreyMap output(input.width(), input.height());
        nop(output, input);
        REQUIRE(output.isEqualTo(input));
    }
}

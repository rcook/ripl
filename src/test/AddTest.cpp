#include "add.h"
#include "catch.hpp"
#include "helper.h"
#include "riplgrey.h"
#include <array>

using namespace std;
using namespace ripl::test::helper;

TEST_CASE("addImages", "[addImages]")
{
    riplGreyMap input0 { 4, 3, { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 } };
    riplGreyMap input1 { 4, 3, { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120 } };

    SECTION("weight 1.0")
    {
        riplGreyMap output { 4, 3 };
        addImages(output, input0, input1, 1.0f);
        REQUIRE(compareImageData(4, 3, { 11, 22, 33, 44, 55, 66, 77, 88, 99, 110, 121, 132 }, output));
    }

    SECTION("weight 2.0")
    {
        riplGreyMap output { 4, 3 };
        addImages(output, input0, input1, 2.0f);
        REQUIRE(compareImageData(4, 3, { 21, 42, 63, 84, 105, 126, 147, 168, 189, 210, 231, 252 }, output));
    }

    SECTION("weight 3.0")
    {
        riplGreyMap output { 4, 3 };
        addImages(output, input0, input1, 3.0f);
        REQUIRE(compareImageData(4, 3, { 31, 62, 93, 124, 155, 186, 217, 248, 255, 255, 255, 255 }, output));
    }
}

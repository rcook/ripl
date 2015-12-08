#include "catch.hpp"
#include "data.h"
#include "helper.h"
#include "riplgrey.h"
#include <array>

using namespace std;
using namespace ripl::test::data;
using namespace ripl::test::helper;

TEST_CASE("Image", "[Image]")
{
    SECTION("basics")
    {
        riplGreyMap image { 4, 3, { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 } };
        REQUIRE(compareImageData(4, 3, { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 }, image));
    }
}

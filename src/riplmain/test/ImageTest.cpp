#include "catch.hpp"
#include "data.h"
#include "riplgrey.h"
#include "testlib/helper.h"
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

    SECTION("hasSameDimensionsAs")
    {
        riplGreyMap image0 { 4, 3, { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 } };
        riplGreyMap image1 { 4, 3, { 10, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 } };
        riplGreyMap image2 { 4, 4, { 10, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 } };
        REQUIRE(image0.hasSameDimensionsAs(image1));
        REQUIRE(!image0.hasSameDimensionsAs(image2));
    }

    SECTION("hasSameDimensionsAs")
    {
        riplGreyMap image0 { 4, 3, { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 } };
        riplGreyMap image1 { 4, 3, { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 } };
        riplGreyMap image2 { 4, 3, { 10, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 } };
        REQUIRE(image0.isEqualTo(image0));
        REQUIRE(image0.isEqualTo(image1));
        REQUIRE(!image0.isEqualTo(image2));
        REQUIRE(image1.isEqualTo(image0));
        REQUIRE(image1.isEqualTo(image1));
        REQUIRE(!image1.isEqualTo(image2));
        REQUIRE(!image2.isEqualTo(image0));
        REQUIRE(!image2.isEqualTo(image1));
        REQUIRE(image2.isEqualTo(image2));
    }
}

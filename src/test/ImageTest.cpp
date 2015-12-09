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
        REQUIRE(Image.data);
        REQUIRE(Image.cols * Image.rows == Image.size);
    }

    SECTION("in-memory")
    {
        array<riplGrey, 12> data = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
        riplGreyMap image { 4, 3, data.size(), data.data() };

        REQUIRE(compareImageData(4, 3, { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 }, image));
    }
}

#include "catch.hpp"
#include "data.h"
#include "riplgrey.h"
#include <array>

using namespace std;
using namespace ripl::test::data;

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

        REQUIRE(image.data);
        REQUIRE(image.cols * image.rows == image.size);
        REQUIRE(image.size == data.size());

        REQUIRE(image.data[0] == 1);
        REQUIRE(image.data[1] == 2);
        REQUIRE(image.data[2] == 3);
        REQUIRE(image.data[3] == 4);
        REQUIRE(image.data[4] == 5);
        REQUIRE(image.data[5] == 6);
        REQUIRE(image.data[6] == 7);
        REQUIRE(image.data[7] == 8);
        REQUIRE(image.data[8] == 9);
        REQUIRE(image.data[9] == 10);
        REQUIRE(image.data[10] == 11);
        REQUIRE(image.data[11] == 12);
    }
}

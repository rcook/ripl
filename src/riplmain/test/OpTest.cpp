#include "Op.h"
#include "catch.hpp"

static int executeFunc(unsigned, const char**, riplGreyMap*, riplGreyMap*)
{
    return 0;
}

static const char* helpFunc()
{
    return nullptr;
}

TEST_CASE("Op", "Op")
{
    SECTION("basics")
    {
        Op op("name", "description", executeFunc, helpFunc);
        REQUIRE(op.name().compare("name") == 0);
        REQUIRE(op.description().compare("description") == 0);
    }
}

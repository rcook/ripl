#include "catch.hpp"
#include "resources.h"
#include "riplfilt.h"
#include "testlib/MemoryBuffer.h"

using namespace std;
using namespace resources;

TEST_CASE("Netpbm", "Netpbm")
{
    SECTION("riplReadGraphicFormat ASCII PGM")
    {
        MemoryBuffer buffer(
            casablanca_ascii_pgm.data(),
            casablanca_ascii_pgm.data() + casablanca_ascii_pgm.size());
        istream stream(&buffer);

        auto format = riplReadGraphicFormat(stream);
        CHECK(format == gfPGMASCII);
    }

    SECTION("riplReadGraphicFormat binary PGM")
    {
        MemoryBuffer buffer(
            lena_pgm.data(),
            lena_pgm.data() + lena_pgm.size());
        istream stream(&buffer);

        auto format = riplReadGraphicFormat(stream);
        CHECK(format == gfPGMBinary);
    }

    SECTION("riplLoadImage ASCII PGM")
    {
        MemoryBuffer buffer(
            casablanca_ascii_pgm.data(),
            casablanca_ascii_pgm.data() + casablanca_ascii_pgm.size());
        istream stream(&buffer);

        auto image = riplLoadImage(stream);
        CHECK(casablanca_dat.size() == image.size());
        CHECK(memcmp(casablanca_dat.data(), image.data(), casablanca_dat.size()) == 0);
    }

    SECTION("riplLoadImage binary PGM")
    {
        MemoryBuffer buffer(
            lena_pgm.data(),
            lena_pgm.data() + lena_pgm.size());
        istream stream(&buffer);

        auto image = riplLoadImage(stream);
        CHECK(lena_dat.size() == image.size());
        CHECK(memcmp(lena_dat.data(), image.data(), lena_dat.size()) == 0);
    }
}

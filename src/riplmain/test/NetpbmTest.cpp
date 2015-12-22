#include "catch.hpp"
#include "resources.h"
#include "riplfilt.h"
#include <cstring>

using namespace std;
using namespace resources;

TEST_CASE("Netpbm", "Netpbm")
{
    SECTION("riplReadGraphicFormat ASCII PGM")
    {
        auto format = riplReadGraphicFormat(casablanca_ascii_pgm.data(), casablanca_ascii_pgm.size());
        CHECK(format == ImageFormat::PgmAscii);
    }

    SECTION("riplReadGraphicFormat binary PGM")
    {
        auto format = riplReadGraphicFormat(lena_pgm.data(), lena_pgm.size());
        CHECK(format == ImageFormat::PgmBinary);
    }

    SECTION("riplLoadImage ASCII PGM")
    {
        auto image = riplLoadImage(casablanca_ascii_pgm.data(), casablanca_ascii_pgm.size());
        CHECK(casablanca_dat.size() == image.size());
        CHECK(memcmp(casablanca_dat.data(), image.data(), casablanca_dat.size()) == 0);
    }

    SECTION("riplLoadImage binary PGM")
    {
        auto image = riplLoadImage(lena_pgm.data(), lena_pgm.size());
        CHECK(lena_dat.size() == image.size());
        CHECK(memcmp(lena_dat.data(), image.data(), lena_dat.size()) == 0);
    }
}


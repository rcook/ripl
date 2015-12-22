#include "catch.hpp"
#include "resources.h"
#include "testlib/logging.h"

using namespace resources;
using namespace ripl::test::logging;

TEST_CASE("nop", "nop")
{
    logBinaryBlob("nop.pgm", lena_pgm.data(), lena_pgm.size());
}

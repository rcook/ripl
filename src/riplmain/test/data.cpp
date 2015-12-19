#include "data.h"

#include "resources.h"
#include "riplfilt.h"
#include "testlib/MemoryBuffer.h"
#include <istream>

using namespace std;
using namespace resources;
using namespace ripl::test::data;

static riplGreyMap loadImageResource(const char* begin, const char* end)
{
    MemoryBuffer buffer(begin, end);
    istream stream(&buffer);
    return riplLoadImage(stream);
}

namespace ripl { namespace test { namespace data
{
    const riplGreyMap Image(loadImageResource(lena_pgm.data(), lena_pgm.data() + lena_pgm.size()));
}}} // namespace ripl::test::data

#include "data.h"

#include "resources.h"
#include "riplfilt.h"
//#include "testlib/MemoryBuffer.h"
//#include <istream>

//using namespace std;
using namespace resources;
//using namespace ripl::test::data;

namespace ripl { namespace test { namespace data
{
    const riplGreyMap Image(riplLoadImage(lena_pgm.data(), lena_pgm.size()));
}}} // namespace ripl::test::data

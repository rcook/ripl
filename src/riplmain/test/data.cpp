#include "data.h"

#include "riplfilt.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace ripl::test::data;

#ifdef _MSC_VER
#define BASE_DIR "../"
#else
#define BASE_DIR ""
#endif

namespace ripl { namespace test { namespace data
{
    const riplGreyMap Image(riplLoadImage(BASE_DIR "scratch/lena.pgm"));
}}} // namespace ripl::test::data


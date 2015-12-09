#include "data.h"

#include "riplfilt.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace ripl::test::data;

//static vector<char> readBinaryFile(const string& fileName);

#ifdef _MSC_VER
#define BASE_DIR "../"
#else
#define BASE_DIR ""
#endif

namespace ripl { namespace test { namespace data
{
    const riplGreyMap Image(*riplLoadImage(BASE_DIR "test/lena.pgm"));
    //const vector<char> ImageData(readBinaryFile("../test/lena.pgm"));
}}} // namespace ripl::test::data

/*
static vector<char> readBinaryFile(const string& fileName)
{
    ifstream f(fileName, ios::in | ios::binary | ios::ate);
    vector<char> data(f.tellg());
    f.seekg(0, ios::beg);
    f.read(data.data(), data.size());
    return data;
}
*/

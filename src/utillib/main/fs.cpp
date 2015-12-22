#include "utillib/fs.h"
#include "validate.h"
#ifdef BUILD_WINDOWS
#include <direct.h>
#endif
#ifdef BUILD_LINUX
#include <sys/stat.h>
#endif

using namespace std;

string getDir(const string& path)
{
    auto pos = path.find_last_of("\\/");
    return pos == string::npos  ? "" : path.substr(0, pos);
}

string joinPaths(const string& path0, const string& path1)
{
#ifdef BUILD_WINDOWS
    return path0 + "\\" + path1;
#else
    return path0 + "/" + path1;
#endif
}

void makeDir(const string& path)
{
#ifdef BUILD_WINDOWS
    UTILLIB_REQUIRE(!mkdir(path.data()));
#else
    UTILLIB_REQUIRE(!mkdir(path.data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH));
#endif
}


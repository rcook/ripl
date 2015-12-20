#include <algorithm>
#ifdef BUILD_LINUX
#include <dirent.h>
#endif
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#ifdef BUILD_WINDOWS
#include <windows.h>
#endif

using namespace std;

static string joinPaths(const string& path0, const string& path1)
{
#if defined(BUILD_LINUX) || defined(BUILD_OSX)
    return path0 + '/' + path1;
#else
    return path0 + '\\' + path1;
#endif
}

#if defined(BUILD_LINUX) || defined(BUILD_OSX)

// Usually, we'd check for DT_REG, but Travis-CI reports DT_UNKNOWN
// for regular files. Instead, we'll consider directory entries that
// are _not_ any of the non-DT_UNKNOWN values.
static bool isRegularFile(const dirent* entry)
{
    auto type = entry->d_type;

    return type != DT_FIFO &&
        type != DT_CHR &&
        type != DT_DIR &&
        type != DT_BLK &&
        type != DT_LNK &&
        type != DT_SOCK &&
        type != DT_WHT;
}

static vector<string> getFileNames(const string& dir)
{
    unique_ptr<DIR, decltype(closedir)*> dirHandle(opendir(dir.data()), closedir);
    if (!dirHandle)
    {
        throw runtime_error("opendir failed");
    }

    vector<string> fileNames;
    dirent* entry;
    while ((entry = readdir(dirHandle.get())))
    {
        if (isRegularFile(entry))
        {
            fileNames.push_back(dir + "/" + entry->d_name);
        }
    }

    return fileNames;
}

#else

static vector<string> getFileNames(const string& dir)
{
    WIN32_FIND_DATAA findData;

    string filter(dir + "\\*.*");
    unique_ptr<remove_pointer<HANDLE>::type, decltype(FindClose)*> findHandle(
        FindFirstFileA(filter.data(), &findData),
        FindClose);
    if (findHandle.get() == INVALID_HANDLE_VALUE)
    {
        throw runtime_error("FindFirstFile failed");
    }

    vector<string> fileNames;
    do
    {
        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            fileNames.push_back(dir + "\\" + findData.cFileName);
        }
    } while (FindNextFileA(findHandle.get(), &findData));

    return fileNames;
}

#endif

struct Hex
{
    Hex(uint8_t value_) : value(value_) { }
    const uint8_t value;
};

static inline ostream& operator<<(ostream& os, Hex hex)
{
    static const char* s_digits = "0123456789abcdef";
    uint8_t msbIndex = (hex.value & 0xf0) >> 4;
    uint8_t lsbIndex = hex.value & 0x0f;
    return os << "0x" << s_digits[msbIndex] << s_digits[lsbIndex];
}

static vector<char> readBinaryFile(const string& fileName)
{
    ifstream f(fileName, ios::in | ios::binary | ios::ate);
    vector<char> data(static_cast<size_t>(f.tellg()));
    f.seekg(0, ios::beg);
    f.read(data.data(), data.size());
    return data;
}

static string makeIdentifier(const string& resourcePath)
{
    auto index = resourcePath.find_last_of("/\\");
    auto identifier = index == string::npos ? resourcePath : resourcePath.substr(index + 1);
    replace(identifier.begin(), identifier.end(), '.', '_');
    return identifier;
}

static void emitResourceArray(ostream& cpp, ostream& h, const string& resourcePath)
{
    auto values = readBinaryFile(resourcePath);

    auto identifier = makeIdentifier(resourcePath);

    cpp << "    const array<char, " << values.size() << "> " << identifier << " = \n";
    cpp << "    {\n";

    h << "    extern const std::array<char, " << values.size() << "> " << identifier << ";\n";

    for (size_t i = 0; i < values.size(); ++i)
    {
        if ((i % 8) == 0)
        {
            cpp << "        ";
        }

        cpp << Hex(values[i]) << ", ";

        if ((i % 8) == 7)
        {
            cpp << '\n';
        }
    }

    cpp << "\n    };\n";
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        cerr << "Invalid command line" << endl;
        return EXIT_FAILURE;
    }

    string resourceDir(argv[1]);
    string outputDir(argv[2]);

    ofstream cpp(joinPaths(outputDir, "resources.cpp"));
    ofstream h(joinPaths(outputDir, "resources.h"));

    cpp << "// Automatically generated using importresources tool\n\n";
    cpp << "#include \"resources.h\"\n\n";
    cpp << "using namespace std;\n\n";
    cpp << "#ifdef BUILD_WINDOWS\n";
    cpp << "#pragma warning(push)\n";
    cpp << "#pragma warning(disable:4309)\n";
    cpp << "#elif BUILD_LINUX\n";
    cpp << "#pragma GCC diagnostic push\n";
    cpp << "#pragma GCC diagnostic ignored \"-Wnarrowing\"\n";
    cpp << "#endif\n\n";
    cpp << "namespace resources\n";
    cpp << "{\n";

    h << "// Automatically generated using importresources tool\n\n";
    h << "#pragma once\n\n";
    h << "#include <array>\n\n";
    h << "namespace resources\n";
    h << "{\n";

    for (const auto& fileName : getFileNames(resourceDir))
    {
        emitResourceArray(cpp, h, fileName);
    }

    cpp << "} // namespace resources\n\n";
    cpp << "#ifdef BUILD_WINDOWS\n";
    cpp << "#pragma warning(pop)\n";
    cpp << "#elif BUILD_LINUX\n";
    cpp << "#pragma GCC diagnostic pop\n";
    cpp << "#endif\n";

    h << "} // namespace resources\n";
}

#include "plugin_helper.h"

#include "utillib/OSError.h"
#include "utillib/ScopedHandle.h"
#include "utillib/fs.h"
#include "utillib/string.h"
#if defined(BUILD_LINUX) || defined(BUILD_OSX)
#include <dirent.h>
#endif
#include <functional>
#ifdef BUILD_WINDOWS
#include <windows.h>
#endif

using namespace std;

#ifdef BUILD_WINDOWS

vector<string> getCandidatePluginFileNames(const string& pluginDir)
{
    using FindHandle = ScopedHandle<HANDLE, INVALID_HANDLE_VALUE, decltype(FindClose)*>;

    vector<string> fileNames;

    string filter(pluginDir + "\\*.dll");
    WIN32_FIND_DATA findData;
    FindHandle handle(FindFirstFile(filter.data(), &findData), FindClose);
    if (!handle)
    {
        DWORD dwError = GetLastError();
        if (dwError == ERROR_FILE_NOT_FOUND || dwError == ERROR_PATH_NOT_FOUND)
        {
            return fileNames;
        }

        OSError::throwError(dwError, "FindFirstFile");
    }

    do
    {
        fileNames.emplace_back(joinPaths(pluginDir, findData.cFileName));
    }
    while (FindNextFile(handle.get(), &findData));

    return fileNames;
}

#else

static vector<string> getFileNames(const string& dir, function<bool(const dirent*)> predicate)
{
    using DirHandle = ScopedHandle<DIR*, nullptr, decltype(closedir)*>;

    vector<string> fileNames;

    DirHandle handle(opendir(dir.data()), closedir);
    if (!handle)
    {
        return fileNames;
    }

    dirent* entry;
    while ((entry = readdir(handle.get())))
    {
        if (predicate(entry))
        {
            fileNames.emplace_back(joinPaths(dir, entry->d_name));
        }
    }

    return fileNames;
}

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

vector<string> getCandidatePluginFileNames(const string& pluginDir)
{
    return getFileNames(pluginDir, [](const dirent* entry)
    {
        return isRegularFile(entry)
            && stringBeginsWith(entry->d_name, "lib")
#ifdef BUILD_LINUX
            && stringEndsWith(entry->d_name, ".so");
#else
            && stringEndsWith(entry->d_name, ".dylib");
#endif
    });
}

#endif

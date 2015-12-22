#define CATCH_CONFIG_RUNNER

#include "Error.h"
#include "catch.hpp"
#include "validate.h"
#include "testlib/logging.h"
#include "utillib/fs.h"
#include <array>
#include <direct.h>
#include <iostream>

using namespace std;
using namespace ripl;
using namespace ripl::test::logging;

class LoggerImpl : public Logger
{
public:
    LoggerImpl() = delete;
    LoggerImpl(const LoggerImpl&) = delete;
    LoggerImpl& operator=(const LoggerImpl&) = delete;

public:
    ~LoggerImpl() = default;

    LoggerImpl(const string& dir);

    void logBinaryBlob(const char* fileName, const char* buffer, size_t bufferSize) override;

private:
    const string m_logDir;
};

int main(int argc, char* const argv[])
{
    string appDir = getDir(argv[0]);
    LoggerImpl logger(appDir);
    registerLogger(logger);

    auto result = Catch::Session().run(argc, argv);
    cout << "unittest end" << endl;
}

string makeLogDir(const string& dir)
{
    time_t rawTime;
    time(&rawTime);
    tm* localTime = localtime(&rawTime);
    array<char, 80> buffer;
    strftime(
        buffer.data(),
        buffer.size(),
        "%Y%m%d%H%M%S",
        localTime);

    string logDir = joinPaths(dir, string("log-") + buffer.data());
    return logDir;
}

LoggerImpl::LoggerImpl(const string& dir)
    : m_logDir(makeLogDir(dir))
{
    RIPL_REQUIRE(!mkdir(m_logDir.data()), error::IOError);
}

void LoggerImpl::logBinaryBlob(const char* fileName, const char* buffer, size_t bufferSize)
{
    string path = joinPaths(m_logDir, fileName);
    ofstream os(path, ios::out | ios::binary);
    RIPL_REQUIRE(os, error::IOError);
    os.write(buffer, bufferSize);
}

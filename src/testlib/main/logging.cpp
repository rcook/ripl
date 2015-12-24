#include "testlib/logging.h"

using namespace std;

namespace ripl { namespace test { namespace logging {
    Logger::Logger()
    {
    }

    Logger* s_logger;

    void registerLogger(Logger& logger)
    {
        s_logger = &logger;
    }

    void logBinaryBlob(const string& fileName, const char* buffer, size_t bufferSize)
    {
        if (s_logger)
        {
            s_logger->logBinaryBlob(fileName, buffer, bufferSize);
        }
    }
}}} // namespace ripl::test::logging


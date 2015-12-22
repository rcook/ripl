#pragma once

namespace ripl { namespace test { namespace logging {
    class Logger
    {
    protected:
        Logger();

    public:
        virtual void logBinaryBlob(const char* fileName, const char* buffer, size_t bufferSize) = 0;
    };

    void registerLogger(Logger& logger);
    void logBinaryBlob(const char* fileName, const char* buffer, size_t bufferSize);
}}} // namespace ripl::test::logging

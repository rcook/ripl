#pragma once

#include <string>

namespace ripl { namespace error
{
    class Error
    {
    public:
        Error() = delete;
        Error(const Error&) = delete;
        Error& operator=(const Error&) = delete;

    public:
        ~Error() = default;

        Error(const std::string& message);

        const std::string& message() const;

    private:
        const std::string m_message;
    };

    extern Error OutOfMemory;
    extern Error InvalidOperation;
    extern Error FileNotFound;
    extern Error EndOfFile;
    extern Error IOError;
    extern Error ImageTooBig;
    extern Error InvalidWidth;
    extern Error InvalidHeight;
    extern Error InvalidSize;
}} // namespace ripl::error

#pragma once

#include <streambuf>

// http://www.mr-edd.co.uk/blog/beginners_guide_streambuf
class MemoryBuffer : public std::streambuf
{
private:
    using ConstCharPtr = const char*;

public:
    MemoryBuffer() = delete;
    MemoryBuffer(const MemoryBuffer&) = delete;
    MemoryBuffer &operator=(const MemoryBuffer&) = delete;

public:
    ~MemoryBuffer() = default;

    MemoryBuffer(ConstCharPtr begin, ConstCharPtr end);

private:
    int_type underflow() override;
    int_type uflow() override;
    int_type pbackfail(int_type c) override;
    std::streamsize showmanyc() override;

private:
    const ConstCharPtr m_begin;
    const ConstCharPtr m_end;
    ConstCharPtr m_current;
};

#include "utillib/MemoryBuffer.h"

#include "validate.h"
#include <functional>

using namespace std;

MemoryBuffer::MemoryBuffer(ConstCharPtr begin, ConstCharPtr end)
    : m_begin(begin)
    , m_end(end)
    , m_current(m_begin)
{
    UTILLIB_VALIDATE_ARG_NAME(less_equal<ConstCharPtr>()(begin, end), "end");
}

MemoryBuffer::int_type MemoryBuffer::underflow()
{
    if (m_current == m_end)
    {
        return traits_type::eof();
    }

    return traits_type::to_int_type(*m_current);
}

MemoryBuffer::int_type MemoryBuffer::uflow()
{
    if (m_current == m_end)
    {
        return traits_type::eof();
    }

    return traits_type::to_int_type(*m_current++);
}

MemoryBuffer::int_type MemoryBuffer::pbackfail(int_type c)
{
    if (m_current == m_begin || (c != traits_type::eof() && c != m_current[-1]))
    {
        return traits_type::eof();
    }

    return traits_type::to_int_type(*--m_current);
}

streamsize MemoryBuffer::showmanyc()
{
    UTILLIB_REQUIRE(less_equal<ConstCharPtr>()(m_current, m_end));
    return m_end - m_current;
}


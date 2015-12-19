#pragma once

#include "utillib/validate.h"

template<typename THandle, THandle InvalidHandleValue, typename TCloseFunc>
class ScopedHandle
{
public:
    ScopedHandle(const ScopedHandle&) = delete;
    ScopedHandle& operator=(const ScopedHandle&) = delete;

public:
    explicit ScopedHandle(TCloseFunc closeFunc)
        : m_handle(InvalidHandleValue)
        , m_closeFunc(closeFunc)
    {
    }

    explicit ScopedHandle(THandle handle, TCloseFunc closeFunc)
        : m_handle(handle)
        , m_closeFunc(closeFunc)
    {
    }

    ScopedHandle(ScopedHandle&& other)
        : m_handle(other.release())
        , m_closeFunc(std::move(other.m_closeFunc))
    {
    }

    ~ScopedHandle()
    {
        if (m_handle != InvalidHandleValue)
        {
            m_closeFunc(m_handle);
            m_handle = InvalidHandleValue;
        }
    }

    operator bool() const { return m_handle != InvalidHandleValue; }

    bool operator!() const { return m_handle == InvalidHandleValue; }

    void swap(ScopedHandle& other)
    {
        using std::swap;

        swap(m_handle, other.m_handle);
    }

    THandle release()
    {
        THandle result = m_handle;
        m_handle = InvalidHandleValue;
        return result;
    }

    void reset(THandle handle = InvalidHandleValue)
    {
        ScopedHandle temp(m_handle, m_closeFunc);
        m_handle = handle;
    }

    THandle get() const
    {
        XYZ_REQUIRE(m_handle != InvalidHandleValue);

        return m_handle;
    }

private:
    THandle m_handle;
    TCloseFunc m_closeFunc;
};


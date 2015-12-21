#pragma once

#include <cstdlib>
#include <cstring>
#include <memory>

#define riplCalloc calloc
#define riplFree free

template<typename T> using Array = std::unique_ptr<T[]>;

template<typename T> Array<T> makeArray(size_t count)
{
    return Array<T>(new T[count]);
}

template<typename T> Array<T> makeArray(size_t count, T fillValue)
{
    auto result = Array<T>(new T[count]);
    memset(result.get(), fillValue, count * sizeof(T));
    return result;
}

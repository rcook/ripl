#pragma once

#include <vector>

namespace ripl
{
    template<typename T>
    class ConstVectorRange
    {
    private:
        using const_iterator = typename std::vector<T>::const_iterator;

    public:
        ConstVectorRange() = delete;
        ConstVectorRange& operator=(const ConstVectorRange&) = delete;

    public:
        ConstVectorRange(const ConstVectorRange&) = default;
        ~ConstVectorRange() = default;

        ConstVectorRange(const_iterator begin, const_iterator end)
            : m_begin(begin)
            , m_end(end) { }

        const_iterator begin() const { return m_begin; }
        const_iterator end() const { return m_end; }

    private:
        const const_iterator m_begin;
        const const_iterator m_end;
    };

    template<typename T>
    class VectorRange
    {
    private:
        using iterator = typename std::vector<T>::iterator;

    public:
        VectorRange() = delete;
        VectorRange& operator=(const VectorRange&) = delete;

    public:
        VectorRange(const VectorRange&) = default;
        ~VectorRange() = default;

        VectorRange(iterator begin, iterator end)
            : m_begin(begin)
            , m_end(end) { }

        iterator begin() { return m_begin; }
        iterator end() { return m_end; }

    private:
        const iterator m_begin;
        const iterator m_end;
    };

    template<typename T>
    class ConstVectorRangeWithStride
    {
    private:
        class Iterator : public std::iterator<std::forward_iterator_tag, T, size_t, T*, T&>
        {
        public:
            Iterator() = delete;
            Iterator& operator=(const Iterator&) = delete;

        public:
            Iterator(const Iterator&) = default;
            ~Iterator() = default;

            Iterator(const std::vector<T>& values, size_t index, size_t stride)
                : m_values(values)
                , m_index(index)
                , m_stride(stride) { }

            bool operator==(const Iterator& other) const { return m_index == other.m_index; }

            bool operator!=(const Iterator& other) const { return !operator==(other); }

            Iterator& operator++()
            {
                m_index += m_stride;
                return *this;
            }

            T operator*() const { return m_values[m_index]; }

        private:
            const std::vector<T>& m_values;
            size_t m_index;
            const size_t m_stride;
        };

    public:
        ConstVectorRangeWithStride() = delete;
        ConstVectorRangeWithStride(const ConstVectorRangeWithStride&) = delete;
        ConstVectorRangeWithStride& operator=(const ConstVectorRangeWithStride&) = delete;

    public:
        ~ConstVectorRangeWithStride() = default;

        ConstVectorRangeWithStride(const std::vector<T>& values, size_t beginIndex, size_t endIndex, size_t stride)
            : m_values(values)
            , m_beginIndex(beginIndex)
            , m_endIndex(endIndex)
            , m_stride(stride) { }

        Iterator begin() const { return Iterator(m_values, m_beginIndex, m_stride); }
        Iterator end() const { return Iterator(m_values, m_endIndex, m_stride); }

    private:
        const std::vector<T>& m_values;
        const size_t m_beginIndex;
        const size_t m_endIndex;
        const size_t m_stride;
    };

    template<typename T>
    class VectorRangeWithStride
    {
    private:
        class Iterator : public std::iterator<std::forward_iterator_tag, T, size_t, T*, T&>
        {
        public:
            Iterator() = delete;
            Iterator& operator=(const Iterator&) = delete;

        public:
            Iterator(const Iterator&) = default;
            ~Iterator() = default;

            Iterator(std::vector<T>& values, size_t index, size_t stride)
                : m_values(values)
                , m_index(index)
                , m_stride(stride) { }

            bool operator==(const Iterator& other) const { return m_index == other.m_index; }

            bool operator!=(const Iterator& other) const { return !operator==(other); }

            Iterator& operator++()
            {
                m_index += m_stride;
                return *this;
            }

            T operator*() const { return m_values[m_index]; }

            T& operator*() { return m_values[m_index]; }

        private:
            std::vector<T>& m_values;
            size_t m_index;
            const size_t m_stride;
        };

    public:
        VectorRangeWithStride() = delete;
        VectorRangeWithStride(const VectorRangeWithStride&) = delete;
        VectorRangeWithStride& operator=(const VectorRangeWithStride&) = delete;

    public:
        ~VectorRangeWithStride() = default;

        VectorRangeWithStride(std::vector<T>& values, size_t beginIndex, size_t endIndex, size_t stride)
            : m_values(values)
            , m_beginIndex(beginIndex)
            , m_endIndex(endIndex)
            , m_stride(stride) { }

        Iterator begin() const { return Iterator(m_values, m_beginIndex, m_stride); }
        Iterator end() const { return Iterator(m_values, m_endIndex, m_stride); }

    private:
        std::vector<T>& m_values;
        const size_t m_beginIndex;
        const size_t m_endIndex;
        const size_t m_stride;
    };

    template<typename T>
    ConstVectorRange<T> range(const std::vector<T>& values, size_t beginIndex, size_t endIndex)
    {
        auto begin = values.begin();
        return ConstVectorRange<T>(
            begin + beginIndex,
            begin + endIndex);
    }

    template<typename T>
    VectorRange<T> range(std::vector<T>& values, size_t beginIndex, size_t endIndex)
    {
        auto begin = values.begin();
        return VectorRange<T>(
            begin + beginIndex,
            begin + endIndex);
    }

    template<typename T>
    ConstVectorRangeWithStride<T> range(const std::vector<T>& values, size_t beginIndex, size_t endIndex, size_t stride)
    {
        return ConstVectorRangeWithStride<T>(
            values,
            beginIndex,
            endIndex,
            stride);
    }

    template<typename T>
    VectorRangeWithStride<T> range(std::vector<T>& values, size_t beginIndex, size_t endIndex, size_t stride)
    {
        return VectorRangeWithStride<T>(
            values,
            beginIndex,
            endIndex,
            stride);
    }
} // namespace ripl

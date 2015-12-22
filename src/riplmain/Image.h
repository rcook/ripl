#pragma once

#include "defs.h"
#include "validate.h"
#include "Ranges.h"
#include <utility>
#include <vector>

namespace ripl
{
    template<typename T>
    class Image
    {
    public:
        using vector = std::vector<T>;
        using const_iterator = typename vector::const_iterator;
        using iterator = typename vector::iterator;

    public:
        Image() = delete;
        Image& operator=(const Image&) = delete;

    public:
        Image(const Image&) = default;
        ~Image() = default;

        Image(image_dimension_t width, image_dimension_t height)
            : m_width(width)
            , m_height(height)
            , m_data(width * height)
        {
            RIPL_VALIDATE_ARG(m_width >= 1, error::InvalidWidth);
            RIPL_VALIDATE_ARG(m_height >= 1, error::InvalidHeight);
        }

        Image(image_dimension_t width, image_dimension_t height, T fillValue)
            : m_width(width)
            , m_height(height)
            , m_data(width * height)
        {
            RIPL_VALIDATE_ARG(m_width >= 1, error::InvalidWidth);
            RIPL_VALIDATE_ARG(m_height >= 1, error::InvalidHeight);

            m_data.assign(m_data.size(), fillValue);
        }

        Image(image_dimension_t width, image_dimension_t height, const vector& data)
            : m_width(width)
            , m_height(height)
            , m_data(data)
        {
            RIPL_VALIDATE_ARG(m_width >= 1, error::InvalidWidth);
            RIPL_VALIDATE_ARG(m_height >= 1, error::InvalidHeight);
            RIPL_VALIDATE_ARG(m_width * m_height == m_data.size(), error::InvalidSize);
        }

        Image(image_dimension_t width, image_dimension_t height, vector&& data)
            : m_width(width)
            , m_height(height)
            , m_data(move(data))
        {
            RIPL_VALIDATE_ARG(m_width >= 1, error::InvalidWidth);
            RIPL_VALIDATE_ARG(m_height >= 1, error::InvalidHeight);
            RIPL_VALIDATE_ARG(m_width * m_height == m_data.size(), error::InvalidSize);
        }

        void swap(Image& other)
        {
            using std::swap;

            swap(m_width, other.m_width);
            swap(m_height, other.m_height);
            m_data.swap(other.m_data);
        }

        image_dimension_t width() const { return m_width; }
        image_dimension_t height() const { return m_height; }
        image_size_t size() const { return m_data.size(); }

        bool hasSameDimensionsAs(const Image& other) const { return m_width == other.m_width && m_height == other.m_height; }

        bool isEqualTo(const Image& other) const
        {
            return hasSameDimensionsAs(other) && m_data == other.m_data;
        }

        const T* data() const { return m_data.data(); }
        T* data() { return m_data.data(); }

        const_iterator begin() const { return m_data.begin(); }
        const_iterator end() const { return m_data.end(); }

        iterator begin() { return m_data.begin(); }
        iterator end() { return m_data.end(); }

        T operator[](image_size_t pos) const { return m_data[pos]; }
        T& operator[](image_size_t pos) { return m_data[pos]; }
        T operator()(image_dimension_t row, image_dimension_t column) const { return m_data[row * m_width + column]; }
        T& operator()(image_dimension_t row, image_dimension_t column) { return m_data[row * m_width + column]; }

        T at(image_size_t pos) const { return m_data.at(pos); }
        T& at(image_size_t pos) { return m_data.at(pos); }
        T at(image_dimension_t row, image_dimension_t column) const { return m_data.at(row * m_width + column); }
        T& at(image_dimension_t row, image_dimension_t column) { return m_data.at(row * m_width + column); }

        ConstVectorRangeWithStride<T> columnAt(image_dimension_t index) const
        {
            return range(m_data, index, m_data.size() + index, m_width);
        }

        VectorRangeWithStride<T> columnAt(image_dimension_t index)
        {
            return range(m_data, index, m_data.size() + index, m_width);
        }

        ConstVectorRange<T> rowAt(image_dimension_t index) const
        {
            return range(m_data, m_width * index, m_width * (index + 1));
        }

        VectorRange<T> rowAt(image_dimension_t index)
        {
            return range(m_data, m_width * index, m_width * (index + 1));
        }

    private:
        image_dimension_t m_width;
        image_dimension_t m_height;
        vector m_data;
    };
} // namespace ripl

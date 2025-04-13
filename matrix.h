#pragma once

#include <vector>
#include <stdexcept>

template <typename T>
class Matrix
{
public:
    Matrix(size_t n, size_t m, T val = {})
        : rows_(n), columns_(m)
    {
        data_.resize(rows_ * columns_, val);
    }

    Matrix(size_t n, size_t m, const std::vector<T>& data)
        : rows_(n), columns_(m)
    {
        if (data.size() != n * m)
        {
            throw std::invalid_argument("Size of data doesn\'t equal rows * columns");
        }

        data_ = data;
    }

    size_t GetRows() const
    {
        return rows_;
    }

    size_t GetColumns() const
    {
        return columns_;
    }

    const std::vector<T> &GetData() const
    {
        return data_;
    }

    const T& operator()(size_t i, size_t j) const
    {
        if (i >= rows_ || j >= columns_)
        {
            throw std::out_of_range("Index out of range");
        }

        return data_[columns_ * i + j];
    }

    T& operator()(size_t i, size_t j)
    {
        if (i >= rows_ || j >= columns_)
        {
            throw std::out_of_range("Index out of rang");
        }

        return data_[columns_ * i + j];
    }

    Matrix operator*(const Matrix &rhs)
    {
        auto lhs = *this;
        if (lhs.columns_ != rhs.rows_)
        {
            throw std::invalid_argument("Wrong matrix dimensions");
        }

        Matrix result(lhs.rows_, rhs.columns_);

        for (size_t i = 0; i < lhs.rows_; ++i)
        {
            for (size_t j = 0; j < rhs.columns_; ++j)
            {
                T cell_value = 0;
                for (size_t k = 0; k < lhs.columns_; ++k)
                {
                    cell_value += lhs(i, k) * rhs(k, j);
                }
                result(i, j) = cell_value;
            }
        }

        return result;
    }

    bool operator==(const Matrix& rhs) const
    {
        return rows_ == rhs.rows_ && columns_ == rhs.columns_ && data_ == rhs.data_;
    }

private:
    size_t rows_;
    size_t columns_;
    std::vector<T> data_;
};

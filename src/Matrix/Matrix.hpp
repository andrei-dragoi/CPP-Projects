#pragma once

#include <iostream>
#include <vector>
#include <format>
#include <thread>

template<typename T>
requires std::is_arithmetic_v<T>
class Matrix
{
public:
    Matrix() = default;
    
    Matrix(const std::vector<std::vector<T>>& matrix)
    : _matrix{matrix}
    {
    }
    
    Matrix(std::vector<std::vector<T>>&& matrix)
    : _matrix{std::move(matrix)}
    {
    }
    
    Matrix(const Matrix& other)
    : _matrix{other._matrix}
    {
    }
    
    Matrix(Matrix&& other)
    : _matrix{std::move(other._matrix)}
    {
    }
    
    Matrix& operator=(const Matrix& other)
    {
        if (this != &other)
        {
            _matrix = other._matrix;
        }
        
        return *this;
    }
    
    Matrix& operator=(Matrix&& other)
    {
        if (this != &other)
        {
            _matrix = std::move(other._matrix);
        }
        
        return *this;
    }
    
    std::vector<T>& operator[](size_t i)
    {
        return _matrix[i];
    }
    
    const std::vector<T>& operator[](size_t i) const
    {
        return _matrix[i];
    }
    
    Matrix& operator+(const Matrix& other)
    {
        if (rows() != other.rows() || cols() != other.cols())
        {
            throw std::runtime_error("Matrices do not have same dimensions");
        }
        
        for (size_t i = 0; i < rows(); ++i)
        {
            for (size_t j = 0; j < cols(); ++j)
            {
                _matrix[i][j] += _matrix[i][j];
            }
        }
        
        return *this;
    }
    
    Matrix& operator-(const Matrix& other)
    {
        if (rows() != other.rows() || cols() != other.cols())
        {
            throw std::runtime_error("Matrices do not have same dimensions");
        }
        
        for (size_t i = 0; i < rows(); ++i)
        {
            for (size_t j = 0; j < cols(); ++j)
            {
                _matrix[i][j] -= _matrix[i][j];
            }
        }
        
        return *this;
    }
    
    Matrix operator*(const Matrix& other) //cache friendly implementation
    {
        if (cols() != other.rows())
        {
            throw std::runtime_error("Matrices do not have dimensions required for multiplication");
        }
        
        std::vector<std::vector<int>> result_vector(rows(), std::vector<int>(other.cols()));
        Matrix result_matrix{std::move(result_vector)};
        
        size_t matrix_half_rows = (rows() >> 1);
        
        std::jthread t1{[&](){ this->multiplication_impl(other, result_matrix, 0, matrix_half_rows); }};
        std::jthread t2{[&](){ this->multiplication_impl(other, result_matrix, matrix_half_rows, rows()); }};
        
        return result_matrix;
    }
    
    bool operator==(const Matrix& other) const
    {
        if (rows() != other.rows() || cols() != other.cols())
        {
            return false;
        }
        
        for (size_t i = 0; i < rows(); ++i)
        {
            for (size_t j = 0; j < cols(); ++j)
            {
                if (_matrix[i][j] != other._matrix[i][j])
                {
                    return false;
                }
            }
        }
        
        return true;
    }
    
    void clear()
    {
        for (auto& row : _matrix)
        {
            row.clear();
        }
    }
    
    size_t rows() const
    {
        return _matrix.size();
    }
    
    size_t cols() const{
        return _matrix[0].size();
    }
    
    void print() const
    {
        for (size_t i = 0; i < rows(); ++i)
        {
            for (size_t j = 0; j < cols(); ++j)
            {
                std::cout << std::format("{} ", _matrix[i][j]);
            }   
            std::cout << '\n';
        }
    }
    
private:
    std::vector<std::vector<T>> _matrix{};
    
    void multiplication_impl(const Matrix& other, Matrix& result_matrix, size_t start_row, size_t end_row)
    {
        for (size_t i = start_row; i < end_row; ++i)
        {
            for (size_t j = 0; j < cols(); ++j)
            {
                for (size_t k = 0; k < other.cols(); ++k)
                {
                    result_matrix[i][k] += _matrix[i][j] * other._matrix[j][k];
                }
            }  
        }
    }
    
    // void slow_multiplication(const Matrix& other)
    // {
    //     if (cols() != other.rows())
    //     {
    //         throw std::runtime_error("Matrices do not have dimensions required for multiplication");
    //     }
        
    //     std::vector<std::vector<int>> result_vector(rows(), std::vector<int>(other.cols()));
    //     Matrix result_matrix{std::move(result_vector)};
        
    //     for (size_t i = 0; i < rows(); ++i)
    //     {
    //         for (size_t j = 0; j < other.cols(); ++j)
    //         {
    //             for (size_t k = 0; k < other.rows(); ++k)
    //             {
    //                 result_matrix[i][j] += _matrix[i][k] * other._matrix[k][j];
    //             }
    //         }   
    //     }
        
    //     return result_matrix;
    // }
};
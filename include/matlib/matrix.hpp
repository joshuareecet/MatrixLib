//
// Created by Joshua Thompson on 26/04/2026.
//

#ifndef MATRIXLIB_MATRIX_HPP
#define MATRIXLIB_MATRIX_HPP

#include <vector>
#include <array>
#include <initializer_list>
#include <cmath>
#include <ostream>
#include <type_traits>
#include <limits>
#include <functional>

namespace matlib {
    enum class Norm {
        Frobenius,
        L1,
        LInf,
    };

    enum class MatrixType {
        Symmetric,
        UpperTriangular,
        LowerTriangular
    };

    template <typename T>
    bool approx_equal(T a, T b, T acceptable_precision = 64 * std::numeric_limits<T>::epsilon()) {
        static_assert(std::is_floating_point_v<T>, "approx_equal only accepts floating point types!");

        // If the numbers are extremely close to zero, we can assume that they are so close that it's a rounding error
        if ((std::abs(a) < acceptable_precision) && (std::abs(b) < 2*acceptable_precision)) {
            return true;
        }

        // For other numbers we check based on the acceptable precision
        if (std::abs(a-b) > acceptable_precision * std::max(std::abs(a),std::abs(b))) {
            return false;
        }

        return true;
    }

    class Matrix {
    public:
        using iterator = std::vector<double>::iterator;
        using const_iterator = std::vector<double>::const_iterator;

    private:
        int rows_ {};
        int cols_ {};
        std::vector<double> mat;

    public:
        Matrix (int r, int c, const std::initializer_list<double>& values);
        Matrix (int r, int c);
        Matrix (int r, int c, const std::vector<double>& vec);

        // move constructor
        Matrix (Matrix&& b) noexcept;
        // move assignment
        Matrix& operator=(Matrix&& b) noexcept;
        // copy constructor
        Matrix (const Matrix& b);
        //copy assignment
        Matrix& operator=(const Matrix& b);
        // destructor
        ~Matrix() = default;

        iterator begin() { return mat.begin(); }
        iterator end() { return mat.end(); }
        const_iterator begin() const { return mat.begin(); }
        const_iterator end() const { return mat.end(); }

        const_iterator cbegin() const { return mat.cbegin(); }
        const_iterator cend() const { return mat.cend(); }

        double* data() { return mat.data(); }
        const double* data() const { return mat.data(); }

        double* row_begin(int i) {
            if (i < 0 || i >= rows_) {
                throw std::out_of_range("ERROR: Row index out of range!");
            }
            return data() + i * cols_;
        }
        double* row_end(int i) {
            if (i < 0 || i >= rows_) {
                throw std::out_of_range("ERROR: Row index out of range!");
            }
            return data() + (i + 1) * cols_;
        }
        const double* row_begin(int i) const {
            if (i < 0 || i >= rows_) {
                throw std::out_of_range("ERROR: Row index out of range!");
            }
            return data() + i * cols_;
        }
        const double* row_end(int i) const {
            if (i < 0 || i >= rows_) {
                throw std::out_of_range("ERROR: Row index out of range!");
            }
            return data() + (i + 1) * cols_;
        }

        [[nodiscard]] std::size_t size() const;
        [[nodiscard]] constexpr int rows() const {return rows_;}
        [[nodiscard]] constexpr int cols() const {return cols_;}

        const double& operator()(int r,int c) const;
        double& operator()(int r, int c);

        [[nodiscard]] Matrix operator+(const Matrix& b) const;
        [[nodiscard]] Matrix operator-(const Matrix& b) const;
        [[nodiscard]] Matrix operator-() const;
        [[nodiscard]] Matrix operator*(double scale) const;
        [[nodiscard]] Matrix operator*(const Matrix& b) const;
        Matrix operator/(const double divisor) const;
        [[nodiscard]] bool operator==(const Matrix& b) const;
        void apply(std::function<double(double)> transformation);

        Matrix column(int i) const;
        void set_column(const Matrix& B, int column);
        void set_column(const std::vector<double>& B, int column);

        Matrix submatrix(int row_start, int col_start, int rows, int cols) const;

        [[nodiscard]] double norm(Norm type = Norm::Frobenius) const;
        [[nodiscard]] Matrix transpose() const;
        friend std::ostream& operator<<(std::ostream& out, const Matrix& mat);
    };
        
    // free function, allows for multiplying double with mat
    Matrix operator*(double scale, const Matrix& b);
    
    // generates identity matrix nxn
    Matrix identity(const int n);
    
    //Generates matrix of zeros
    Matrix zeros(const int n);
    Matrix zeros(const int rows, const int cols);


    // allows for outputting matrix to a stream
    std::ostream& operator<<(std::ostream& out, const Matrix& mat);
    constexpr double det2x2(const std::array<double, 4>& A) {return (A[0]*A[3])-(A[1]*A[2]);}
}
#endif //MATRIXLIB_MATRIX_HPP

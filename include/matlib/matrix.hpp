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
    bool approx_equal(T a, T b, double acceptable_precision = 64 * std::numeric_limits<T>::epsilon()) {
        static_assert(std::is_floating_point_v<T>, "approx_equal only accepts floating point types!");

        // If the numbers are extremely close to zero, we can assume that they are so close that it's a rounding error
        if ((std::abs(a) < acceptable_precision) && (std::abs(b) < acceptable_precision)) {
            return true;
        }

        // For other numbers we check based on the acceptable precision
        if (std::abs(a-b) > acceptable_precision * std::max(std::abs(a),std::abs(b))) {
            return false;
        }

        return true;
    }

    class Matrix {
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

        [[nodiscard]] std::size_t size() const;
        [[nodiscard]] constexpr int rows() const {return rows_;}
        [[nodiscard]] constexpr int cols() const {return cols_;}

        const double& operator()(int r,int c) const;
        double& operator()(int r, int c);

        [[nodiscard]] Matrix operator+(const Matrix& b) const;
        [[nodiscard]] Matrix operator-(const Matrix& b) const;
        [[nodiscard]] Matrix operator*(double scale) const;
        [[nodiscard]] Matrix operator*(const Matrix& b) const;
        [[nodiscard]] bool operator==(const Matrix& b) const;
        [[nodiscard]] Matrix transpose() const;
        friend std::ostream& operator<<(std::ostream& out, const Matrix& mat);
    };

    // free function, allows for multiplying double with mat
    Matrix operator*(double scale, const Matrix& b);
    // generates identity matrix nxn
    Matrix identity(int n);
    // allows for outputting matrix to a stream
    std::ostream& operator<<(std::ostream& out, const Matrix& mat);
    constexpr double det2x2(const std::array<double, 4>& A) {return (A[0]*A[3])-(A[1]*A[2]);}

}
#endif //MATRIXLIB_MATRIX_HPP

//
// Created by Joshua Thompson on 26/04/2026.
//

#include <matlib/matrix.hpp>
#include <iomanip>
#include <limits>

static_assert(sizeof(double) == 8, "double must be 8 bytes");

// helper function for constructors
namespace {
    void constructor_rc_check(const int r,const int c) {
        // no matrix where rows or cols are 0
        if (r <= 0 || c <= 0) {
            throw std::invalid_argument("ERROR: Num rows and columns must be above 0!");
        }
        // check for r*c overflowing max int val
        if (r > std::numeric_limits<int>::max() / c) {
            throw std::overflow_error("ERROR: rows*cols exceeds int value limits");
        }
    }
    void check_nan_inf(const std::vector<double>& vec) {
        for (double val : vec) {
            if (std::isnan(val) || std::isinf(val)) {
                throw std::invalid_argument("ERROR: Matrix data contains NaN or Inf values!");
            }
        }
    }
}

namespace matlib {
    //Matrix initializers

    // construct by r, c, and initialiser list
    Matrix::Matrix(const int r, const int c, const std::initializer_list<double>& values)
        : rows_{r}, cols_{c}, mat{values} {
        constructor_rc_check(r,c);
        check_nan_inf(mat);
        if (r*c != static_cast<int>(values.size())) {
            throw std::invalid_argument("ERROR: Mismatch between r,c and initialiser list");
        }
    }
    // construct via r,c and vector
    Matrix::Matrix(const int r, const int c, const std::vector<double>& vec)
        : rows_{r}, cols_{c}, mat{vec} {
        constructor_rc_check(r,c);
        check_nan_inf(mat);
        if (r*c != static_cast<int>(vec.size())) {
            throw std::invalid_argument("ERROR: Mismatch between r,c and vector size");
        }
    }
    //construct by r and c
    Matrix::Matrix(const int r, const int c)
        : rows_{r}, cols_{c} {
        constructor_rc_check(r,c);
        mat.resize(r*c,0);
    }

    // copy constructor
    Matrix::Matrix(const Matrix& b)
        : rows_{b.rows_}, cols_{b.cols_}, mat{b.mat}
    {}
    // copy assignment constructor
    Matrix& Matrix::operator=(const Matrix& b) {
        // check if memory address is equal first
        if (this == &b) return *this;

        rows_ = b.rows_;
        cols_ = b.cols_;
        mat = b.mat;
        return *this;
    }

    // move assignment constructor
    Matrix::Matrix(Matrix&& b) noexcept
        : rows_ {b.rows_}, cols_{b.cols_}, mat{std::move(b.mat)}
    {
        b.rows_ = 0;
        b.cols_ = 0;
    }

    // move assignment
    Matrix& Matrix::operator=(Matrix&& b) noexcept {
        rows_ = b.rows_;
        cols_ = b.cols_;
        mat = std::move(b.mat);
        b.rows_ = 0;
        b.cols_ = 0;
        b.mat = {};
        return *this;
    }

    //general purpose functions -----------------------------------
    [[nodiscard]] std::size_t Matrix::size() const{return mat.size(); }
    [[nodiscard]] int Matrix::rows() const {return rows_;}
    [[nodiscard]] int Matrix::cols() const {return cols_; }


    // const indexing, allows indexing const matrices
    const double& Matrix::operator()(const int r,const int c) const{
        if (r >= rows_ || r < 0 || c >= cols_ || c < 0) {
            throw std::out_of_range("ERROR: Out of bounds indexing!");
        }
        return mat[r*cols_+c];
    }
    double& Matrix::operator()(const int r, const int c) {
        if (r >= rows_ || r < 0 || c >= cols_ || c < 0) {
            throw std::out_of_range("ERROR: Out of bounds indexing!");
        }
        return mat[r*cols_+c];
    }

    // arithmetic on matrices -------------------------------------

    // Operator overloads
    [[nodiscard]] Matrix Matrix::operator+(const Matrix& b) const{
        if (rows_ != b.rows_ || cols_ != b.cols_) {
            throw std::invalid_argument("ERROR: Matrices are not of compatible size!\n");
        }
        Matrix result(rows_,cols_);
        for (int i{}; i < static_cast<int>(size());++i) {
            result.mat[i] = mat[i] + b.mat[i];
        }
        return result;
    }
    [[nodiscard]] Matrix Matrix::operator-(const Matrix& b) const{
        if (rows_ != b.rows_ || cols_ != b.cols_) {
            throw std::invalid_argument("ERROR: Matrices are not of compatible size!\n");
        }
        Matrix result(rows_,cols_);
        for (int i{}; i < static_cast<int>(size());++i) {
            result.mat[i] = mat[i] - b.mat[i];
        }
        return result;
    }
    // scalar multiply
    [[nodiscard]] Matrix Matrix::operator*(const double scale) const {
        Matrix result(rows_,cols_);
        for (int i{};i < static_cast<int>(size());++i) {
            result.mat[i] = mat[i] * scale;
        }
        return result;
    }
    // allows scale to be first operator
    [[nodiscard]] Matrix operator*(const double scale,const Matrix& b) {
        return b * scale;
    }

    // matmul
    [[nodiscard]] Matrix Matrix::operator*(const Matrix& b) const {
        if ((cols_ != b.rows_)) {
            throw std::invalid_argument("ERROR: Matrix dimensions are mismatched!\n");
        }
        Matrix result(rows_,b.cols_);
        for (int i{};i < rows_;++i) {
            for (int j{};j < b.cols_;++j) {
                for (int k{}; k < cols_;++k) {
                    result(i,j) += (*this)(i,k) * b(k,j);
                }
            }
        }
        return result;
    }


    bool Matrix::operator==(const Matrix& b) const {
        if ((b.rows_ != rows_) || (b.cols_ != cols_)) {
            return false;
        }
        for (int i{}; i < static_cast<int>(size()); ++i) {
            if (!approx_equal(mat[i],b.mat[i],precision)) {
                return false;
            }
        }

        return true;
    }

    // transpose
    [[nodiscard]] Matrix Matrix::transpose() const {
        Matrix result(cols_,rows_);
        // loop through the rows, turn them into columns
        for (int i{}; i<rows_; ++i) {
            for (int j{};j < cols_;++j) {
                result(j,i) = (*this)(i,j);
            }
        }
        return result;
    }

    std::ostream& operator<<(std::ostream& out, const Matrix& mat){
        for (int i {}; i < mat.rows_; ++i) {
            for (int j{}; j< mat.cols_; ++j) {
                out << std::setw(4) << mat(i,j);
            }
            out << '\n';
        }
        out << '\n';
        return out;
    }

    Matrix identity(int n) {
        Matrix I{n,n};
        for (int i{}; i < n; ++i) {
            I(i,i) = 1.0;
        }
        return I;
    }
}
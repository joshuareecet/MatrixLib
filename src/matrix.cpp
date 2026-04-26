//
// Created by Joshua Thompson on 26/04/2026.
//

#include <matlib/matrix.hpp>
#include <iomanip>

namespace matlib {
    //Matrix initializers

    Matrix::Matrix(const int r, const int c, const std::initializer_list<double>& values)
        : rows_{r}, cols_{c}, mat{values} {
        if (r*c != static_cast<int>(values.size())) {
            throw std::invalid_argument("ERROR: Mismatch between r,c and initialiser list");
        }
    }
    Matrix::Matrix(const int r, const int c)
        : rows_{r}, cols_{c}, mat(r*c,0)
    {}
    Matrix::Matrix(const int r, const int c, const std::vector<double>& vec)
        : rows_{r}, cols_{c}, mat{vec} {
        if (r*c != static_cast<int>(vec.size())) {
            throw std::invalid_argument("ERROR: Mismatch between r,c and vector size");
        }
    }


    //general purpose functions -----------------------------------
    [[nodiscard]] std::size_t Matrix::size() const{return mat.size(); }
    [[nodiscard]] int Matrix::rows() const {return rows_;}
    [[nodiscard]] int Matrix::cols() const {return cols_; }

    Matrix operator*(const double scale,const Matrix& b) {
        return b * scale;
    }


    // const indexing, allows indexing const matrices
    const double& Matrix::operator()(const int r,const int c) const{
        return mat[r*cols_+c];
    }
    double& Matrix::operator()(const int r, const int c) {
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
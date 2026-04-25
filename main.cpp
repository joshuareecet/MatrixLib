#include <iostream>
#include <vector>
#include <iomanip>

class Matrix {
private:
    int rows{};
    int cols{};
    std::vector<double> mat;

public:
    Matrix(const int r, const int c, const std::initializer_list<double>& values)
        : rows{r}, cols{c}, mat{values}
    {}
    Matrix(const int r, const int c, const std::vector<double>& vec)
        : rows{r}, cols{c}, mat{vec}
    {}
    Matrix(const int r, const int c)
        : rows{r}, cols{c}, mat(r*c,0)
    {}

    //general purpose functions -----------------------------------
    [[nodiscard]] int size() const{
        return static_cast<int>(mat.size());
    }
    //general purpose functions -----------------------------------
    [[nodiscard]] int num_rows() const{
        return rows;
    }
    //general purpose functions -----------------------------------
    [[nodiscard]] int num_cols() const{
        return cols;
    }

    // const indexing, allows indexing const matrices
    const double& operator()(const int row,const int column) const{
        return mat[row*cols+column];
    }
    double& operator()(const int row, const int column) {
        return mat[row*cols+column];
    }

    // arithmetic on matrices -------------------------------------

    // add and subtract
    [[nodiscard]] Matrix operator+(const Matrix& b) const{
        if (rows != b.rows || cols != b.cols) {
            throw std::invalid_argument("ERROR: Matrices are not of compatible size!\n");
        }
        Matrix result(rows,cols);
        for (int i{}; i < size();++i) {
            result.mat[i] = mat[i] + b.mat[i];
        }
        return result;
    }
    [[nodiscard]] Matrix operator-(const Matrix& b) const{
        if (rows != b.rows || cols != b.cols) {
            throw std::invalid_argument("ERROR: Matrices are not of compatible size!\n");
        }
        Matrix result(rows,cols);
        for (int i{}; i < size();++i) {
            result.mat[i] = mat[i] - b.mat[i];
        }
        return result;
    }
    // scalar multiply
    [[nodiscard]] Matrix operator*(const double scale) const {
        Matrix result(rows,cols);
        for (int i{};i < size();++i) {
            result.mat[i] = mat[i] * scale;
        }
        return result;
    }
    [[nodiscard]] Matrix operator*(const Matrix& b) const {
        if ((cols != b.rows)) {
            throw std::invalid_argument("ERROR: Matrix dimensions are mismatched!\n");
        }
        Matrix result(rows,b.cols);
        for (int i{};i < rows;++i) {
            for (int j{};j < b.cols;++j) {
                for (int k{}; k < cols;++k) {
                    result(i,j) += (*this)(i,k) * b(k,j);
                }
            }
        }

        return result;
    }
};

Matrix operator*(const double scale, const Matrix& mat) {
    return mat * scale;
}

std::ostream& operator<<(std::ostream& out, const Matrix& mat){
    for (int i {}; i < mat.num_rows(); ++i) {
        for (int j{}; j< mat.num_cols(); ++j) {
            out << std::setw(4) << mat(i,j);
        }
        out << '\n';
    }
    out << '\n';
    return out;
}

int main() {
    Matrix A(2, 2, {1, 2, 3, 4});
    Matrix B(2, 2, {5, 6, 7, 8});

    std::cout << "A + B:\n" << A + B << '\n';
    std::cout << "A * B:\n" << A * B << '\n';
    std::cout << "A * 3:\n" << A * 3 << '\n';
    std::cout << "A(0,1) = " << A(0, 1) << '\n';

    return 0;
}
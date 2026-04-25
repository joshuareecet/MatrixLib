#include <iostream>
#include <vector>
#include <iomanip>

struct Matrix {
    int rows{};
    int cols{};
    std::vector<double> mat;

    Matrix(const int r, const int c, const std::initializer_list<double>& values)
        : rows{r}, cols{c}, mat{values}
    {}
    Matrix(const int r, const int c, const std::vector<double>& vec)
        : rows{r}, cols{c}, mat{vec}
    {}
    Matrix(const int r, const int c)
        : rows{r}, cols{c}, mat(r*c,0)
    {}

    [[nodiscard]] double get(const int row, const int col) const{
        return mat[row*cols+col];
    }
    void set(int row, int col, double val) {
        mat[row*cols+col] = val;
    }

    void print() const{
        for (int i {}; i < rows; ++i) {
            for (int j{}; j< cols; ++j) {
                std::cout << std::setw(4) << get(i,j);
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }

    [[nodiscard]] int size() const{
        return static_cast<int>(mat.size());
    }

    [[nodiscard]] Matrix add(const Matrix& mat2) const{
        if ((cols != mat2.cols) || (rows != mat2.rows)){
            throw std::invalid_argument("Matrices must be the same size for element-wise addition!");
        }
        Matrix res {rows,cols};
        for (int i{};i < size(); ++i) {
            res.mat[i] = mat[i]+mat2.mat[i];
        }
        return res;
    }
    [[nodiscard]] Matrix subtract(const Matrix& mat2) const{
        if ((cols != mat2.cols) || (rows != mat2.rows)){
            throw std::invalid_argument("Matrices must be the same size for element-wise subtraction!");
        }
        Matrix res {rows,cols};
        for (int i{};i < size(); ++i) {
            res.mat[i] = mat[i]-mat2.mat[i];
        }
        return res;
    }

    [[nodiscard]] Matrix scalar_multiply(const double scale) const{
        Matrix res(rows,cols);
        for (int i{}; i < size();i++) {
            res.mat[i] = mat[i] * scale;
        }
        return res;
    }

    [[nodiscard]] Matrix multiply(const Matrix& mat2) const{
        if (cols != mat2.rows) {
            throw std::invalid_argument("The dimensions of mat1 and mat2 are mismatched!");
        }
        Matrix result(rows,mat2.cols);

        for (int i{}; i < rows; ++i) {
            for (int j{}; j < mat2.cols; ++j) {
                for (int k{}; k < cols; ++k) {
                    result.mat[i * mat2.cols + j] += get(i, k) * mat2.get(k, j);
                }
            }
        }
        return result;
    }
};


int main() {
    int rows = 2, cols = 2;
    std::vector<double> a_in = {1, 2,
                             3, 4};
    Matrix a(2,2,a_in);
    std::vector<double> b_in = {5, 6,
                             7, 8};
    Matrix b(2,2,b_in);

    std::cout << "A:\n";
    a.print();

    std::cout << "B:\n";
    b.print();

    std::cout << "A + B:\n";
    auto sum = a.add(b);
    sum.print();

    std::cout << "A - B:\n";
    auto diff = a.subtract(b);
    diff.print();

    std::cout << "A * 3:\n";
    auto scaled = a.scalar_multiply(3);
    scaled.print();

    std::cout << "A * B:\n";
    auto product = a.multiply(b);
    product.print();

    // non-square test
    Matrix c{2, 3, {1, 2, 3,
                    4, 5, 6}};

    Matrix d{3, 2, {7,  8,
                    9,  10,
                    11, 12}};

    std::cout << "C (2x3):\n";
    c.print();

    std::cout << "D (3x2):\n";
    d.print();

    std::cout << "C * D:\n";
    c.multiply(d).print();

    return 0;
}
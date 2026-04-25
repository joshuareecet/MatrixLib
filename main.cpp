#include <iostream>
#include <vector>
#include <iomanip>

template <typename T>
bool approx_equal(T a, T b) {
    static_assert(std::is_floating_point_v<T>, "approx_equal only accepts floating point types!");

    double acceptable_precision {1e-13};
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
    int rows{};
    int cols{};
    double epsilon{std::numeric_limits<double>::epsilon()};
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
    Matrix(const Matrix& a)
        : rows{a.rows}, cols{a.cols}, mat{a.mat}
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

    // Operator overloads
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
    bool operator==(const Matrix& b) const {
        if ((b.rows != rows) || (b.cols != cols)) {
            return false;
        }
        for (int i{}; i < size(); ++i) {
            if (!approx_equal(mat[i],b.mat[i])) {
                return false;
            }
        }

        return true;
    }

    // transpose
    [[nodiscard]] Matrix transpose() const {
        Matrix result(cols,rows);
        // loop through the rows, turn them into columns
        for (int i{}; i<rows; ++i) {
            for (int j{};j < cols;++j) {
                result(j,i) = (*this)(i,j);
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

void check(const bool condition, const std::string& name) {
    if (condition) {
        std::cout << "PASS: " << name << '\n';
    }
    else {
        std::cout << "FAIL: " << name << '\n';
    }
}

Matrix identity(int n) {
    Matrix I{n,n};
    for (int i{}; i < n; ++i) {
        I(i,i) = 1.0;
    }
    return I;
}

int main() {
    const Matrix a {2,2,{1,2,3,4}};
    const Matrix b {2,2,{5,6,7,8}};
    const Matrix diff {2,2,{4,4,4,4}};
    const Matrix non_square {2,3,{1,2,3,4,5,6}};
    const Matrix non_square_t {3,2,{1,4,2,5,3,6}};

    check(a*identity(a.num_cols()) == a,"identity");
    check(a + diff == b,"addition");
    check(b - diff == a,"subtraction");
    check(2*a == Matrix (2,2,{2,4,6,8}),"Multiplication");
    check(a.transpose().transpose() == a,"a^T^T = a");
    check(non_square.transpose() == non_square_t,"transpose non square");
    check((a-b).transpose() == (-1*(b-a)).transpose(),"Mixed properties");
    check(a * b == Matrix(2,2,{19,22,43,50}), "matrix multiplication");

    try {
        Matrix c{a+non_square};
        std::cout << "FAIL: did not catch mismatch dimension error!" << '\n';
    }
    catch (const std::exception& e){
        std::cout << "PASS: threw length error for adding square and non-square matrix:\t" << e.what() <<'\n';
    }
}
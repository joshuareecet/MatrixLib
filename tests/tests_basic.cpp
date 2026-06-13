#include <iostream>
#include <algorithm>
#include <iomanip>
#include <matlib/matrix.hpp>
#include <catch2/catch_test_macros.hpp>

const matlib::Matrix a {2,2,{1,2,3,4}};
const matlib::Matrix b {2,2,{5,6,7,8}};
const matlib::Matrix diff {2,2,{4,4,4,4}};
const matlib::Matrix non_square {2,3,{1,2,3,4,5,6}};
const matlib::Matrix non_square_t {3,2,{1,4,2,5,3,6}};

TEST_CASE("Array determinant") {
    constexpr std::array<double, 4> m = {1.0, 2.0, 3.0, 4.0};
    REQUIRE(matlib::det2x2(m) == -2.0);
    static_assert(matlib::det2x2(m) == -2.0, "compile-time det2x2 failed");
}

TEST_CASE("Read Only Matrix", "[Matrix]") {
    const matlib::Matrix const_mat {2,2,{1,2,3,4}};
    REQUIRE(const_mat.rows() == 2);
    REQUIRE(const_mat.cols() == 2);
    REQUIRE(const_mat(0,0) == 1.0);
    REQUIRE((const_mat + const_mat)(0,0) == 2.0);
    REQUIRE((2 * const_mat)(0,0) == 2.0);
    REQUIRE(const_mat == const_mat);
}

TEST_CASE("Matrix free functions", "[Matrix]") {
    // identity matrix
    const matlib::Matrix m {matlib::identity(5)};
    REQUIRE(m.rows() == m.cols());
    REQUIRE(m(3,3) == 1);
    
    //zeros matrix
    const matlib::Matrix z{ matlib::zeros(2,3) };
    const matlib::Matrix z_sq{ matlib::zeros(2) };
    REQUIRE(z.rows() != z.cols());
    REQUIRE(z_sq.rows() == z_sq.cols());
    REQUIRE(std::all_of(z.begin(), z.end(), [](auto x) {return x == 0; }));
    REQUIRE(std::all_of(z_sq.begin(), z_sq.end(), [](auto x) {return x == 0; }));
}

TEST_CASE("Move vs Copy performance", "[performance]") {
    const int size = 1000;
    matlib::Matrix big(size, size);

    // fill with some data
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            big(i, j) = i * size + j;

    // time copy
    auto copy_start = std::chrono::high_resolution_clock::now();
    matlib::Matrix copied = big;
    auto copy_end = std::chrono::high_resolution_clock::now();

    // time move
    auto move_start = std::chrono::high_resolution_clock::now();
    matlib::Matrix moved = std::move(big);
    auto move_end = std::chrono::high_resolution_clock::now();

    auto copy_time = std::chrono::duration_cast<std::chrono::nanoseconds>(copy_end - copy_start).count();
    auto move_time = std::chrono::duration_cast<std::chrono::nanoseconds>(move_end - move_start).count();

    std::cout << "Copy: " << copy_time << " ns\n";
    std::cout << "Move: " << move_time << " ns\n";

    // move should be dramatically faster
    REQUIRE(move_time*100 < copy_time);

    // verify moved-from state
    REQUIRE(big.rows() == 0);
    REQUIRE(big.cols() == 0);
    REQUIRE(big.size() == 0);

    // verify moved-to has the data
    REQUIRE(moved.rows() == size);
    REQUIRE(moved.cols() == size);
}

TEST_CASE("Matrix Class","[Matrix]") {
    static_assert(std::is_nothrow_move_constructible_v<matlib::Matrix>);
    static_assert(std::is_nothrow_move_assignable_v<matlib::Matrix>);

    SECTION("Matrix initialisation") {
        matlib::Matrix test_matrix {2,3,{1,2,3,4,5,6}};
        std::vector<double> vec {1e6,2e6,3e6,4e6};
        matlib::Matrix vec_m {2,2,vec};

        REQUIRE(test_matrix.rows() == 2);
        REQUIRE(test_matrix.cols() == 3);
        REQUIRE(vec_m.rows() == 2);
        REQUIRE(vec_m.cols() == 2);
    } // 4

    SECTION("Matrix exceptions") {
        const std::vector<double> vec {1,2,3,4};
        const matlib::Matrix A {1,1,{1}};
        // sum of non-matching sized matrices
        REQUIRE_THROWS_AS(matlib::Matrix {a + non_square_t},std::invalid_argument);
        // difference between non-matching sized matrices
        REQUIRE_THROWS_AS(matlib::Matrix {a - non_square_t},std::invalid_argument);
        // init list larger than r*c
        REQUIRE_THROWS_AS(matlib::Matrix (2,2,{1,2,3,4,5,6,7,8,9,10}),std::invalid_argument);
        // vec larger than r*c
        REQUIRE_THROWS_AS(matlib::Matrix (1,1,vec),std::invalid_argument);
        // sum of matrix with mismatch dimensions
        REQUIRE_THROWS_AS(matlib::Matrix {a+non_square},std::invalid_argument);
        // Inf in matrix
        REQUIRE_THROWS_AS(matlib::Matrix (1, 1, {std::numeric_limits<double>::infinity()}),std::invalid_argument);
        // NaN in matrix
        REQUIRE_THROWS_AS(matlib::Matrix (1,1,{std::numeric_limits<double>::quiet_NaN()}),std::invalid_argument);
        // out of bounds index
        REQUIRE_THROWS_AS(A(2,3),std::out_of_range);
    } // 8

    SECTION("Matrix operations") {
        REQUIRE(a == matlib::Matrix {2,2,{1,2,3,4}});
        REQUIRE(a-a == matlib::Matrix {2,2,{0,0,0,0}});
        REQUIRE(a + matlib::Matrix{2,2,{0,0,0,0}} == a);
        REQUIRE(a+diff==b);
        REQUIRE(b-diff==a);
        REQUIRE(a*matlib::identity(a.cols()) == a);
        REQUIRE(2*a == matlib::Matrix (2,2,{2,4,6,8}));
        REQUIRE(a*2 == matlib::Matrix (2,2,{2,4,6,8}));
        REQUIRE(a * b == matlib::Matrix(2,2,{19,22,43,50}));
        REQUIRE((a-b).transpose() == (-1*(b-a)).transpose());
    } // 10
}

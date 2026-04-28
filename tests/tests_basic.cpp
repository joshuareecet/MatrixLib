#include <iostream>
#include <iomanip>
#include <matlib/matrix.hpp>
#include <catch2/catch_test_macros.hpp>

const matlib::Matrix a {2,2,{1,2,3,4}};
const matlib::Matrix b {2,2,{5,6,7,8}};
const matlib::Matrix diff {2,2,{4,4,4,4}};
const matlib::Matrix non_square {2,3,{1,2,3,4,5,6}};
const matlib::Matrix non_square_t {3,2,{1,4,2,5,3,6}};

TEST_CASE("Matrix Class","[Matrix]") {
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

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
    }

    SECTION("Matrix exceptions") {
        const std::vector<double> vec {1,2,3,4};

        REQUIRE_THROWS(matlib::Matrix {a + non_square_t});
        REQUIRE_THROWS(matlib::Matrix (2,2,{1,2,3,4,5,6,7,8,9,10}));
        REQUIRE_THROWS(matlib::Matrix (1,1,vec));
        REQUIRE_THROWS(matlib::Matrix {a+non_square});
    }

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
    }
}

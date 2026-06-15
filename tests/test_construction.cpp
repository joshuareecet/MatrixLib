#include <matlib/matrix.hpp>
#include <catch2/catch_test_macros.hpp>
#include <limits>
#include "test_fixtures.hpp"

TEST_CASE("Matrix Class Basics", "[Matrix]") {
	static_assert(std::is_nothrow_move_constructible_v<matlib::Matrix>);
	static_assert(std::is_nothrow_move_assignable_v<matlib::Matrix>);

	SECTION("Matrix initialisation") {
		matlib::Matrix test_matrix{2, 3, {1, 2, 3, 4, 5, 6}};
		std::vector<double> vec{1e6, 2e6, 3e6, 4e6};
		matlib::Matrix vec_m{2, 2, vec};

		REQUIRE(test_matrix.rows() == 2);
		REQUIRE(test_matrix.cols() == 3);
		REQUIRE(vec_m.rows() == 2);
		REQUIRE(vec_m.cols() == 2);
	}

	SECTION("Matrix exceptions") {
		const std::vector<double> vec{1, 2, 3, 4};
		const matlib::Matrix A{1, 1, {1}};
		// sum of non-matching sized matrices
		REQUIRE_THROWS_AS(matlib::Matrix{test_fixtures::a + test_fixtures::non_square_t}, std::invalid_argument);
		// difference between non-matching sized matrices
		REQUIRE_THROWS_AS(matlib::Matrix{test_fixtures::a - test_fixtures::non_square_t}, std::invalid_argument);
		// init list larger than r*c
		REQUIRE_THROWS_AS(matlib::Matrix(2, 2, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}), std::invalid_argument);
		// vec larger than r*c
		REQUIRE_THROWS_AS(matlib::Matrix(1, 1, vec), std::invalid_argument);
		// sum of matrix with mismatch dimensions
		REQUIRE_THROWS_AS(matlib::Matrix{test_fixtures::a + test_fixtures::non_square}, std::invalid_argument);
		// Inf in matrix
		REQUIRE_THROWS_AS(matlib::Matrix(1, 1, {std::numeric_limits<double>::infinity()}), std::invalid_argument);
		// NaN in matrix
		REQUIRE_THROWS_AS(matlib::Matrix(1, 1, {std::numeric_limits<double>::quiet_NaN()}), std::invalid_argument);
		// out of bounds index
		REQUIRE_THROWS_AS(A(2, 3), std::out_of_range);
	}
}

TEST_CASE("Read Only Matrix", "[Matrix]") {
	const matlib::Matrix const_mat{2, 2, {1, 2, 3, 4}};
	REQUIRE(const_mat.rows() == 2);
	REQUIRE(const_mat.cols() == 2);
	REQUIRE(const_mat(0, 0) == 1.0);
	REQUIRE((const_mat + const_mat)(0, 0) == 2.0);
	REQUIRE((2 * const_mat)(0, 0) == 2.0);
	REQUIRE(const_mat == const_mat);
}

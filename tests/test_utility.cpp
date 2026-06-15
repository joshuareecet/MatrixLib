#include <matlib/matrix.hpp>
#include <catch2/catch_test_macros.hpp>
#include <numeric>
#include <algorithm>

TEST_CASE("Apply transformation", "[Matrix]") {
	matlib::Matrix m{2, 2, {1, 2, 3, 4}};
	m.apply([](double x) { return x * 2.0; });
	REQUIRE(m == matlib::Matrix{2, 2, {2, 4, 6, 8}});
}

TEST_CASE("Column extraction", "[Matrix]") {
	matlib::Matrix m{3, 3, {1, 2, 3, 4, 5, 6, 7, 8, 9}};
	auto col1 = m.column(1);
	REQUIRE(col1.rows() == 3);
	REQUIRE(col1.cols() == 1);
	REQUIRE(col1 == matlib::Matrix{3, 1, {2, 5, 8}});

	// invalid column index
	REQUIRE_THROWS_AS(m.column(3), std::invalid_argument);
	REQUIRE_THROWS_AS(m.column(-1), std::invalid_argument);
}

TEST_CASE("Division by scalar", "[Matrix]") {
	matlib::Matrix m{2, 2, {2, 4, 6, 8}};
	auto d = m / 2.0;
	REQUIRE(d == matlib::Matrix{2, 2, {1, 2, 3, 4}});

	REQUIRE_THROWS_AS(m / 0.0, std::invalid_argument);
}

TEST_CASE("Size and iterators", "[Matrix]") {
	matlib::Matrix m{2, 3, {1, 2, 3, 4, 5, 6}};
	REQUIRE(m.size() == 6);
	double sum = std::accumulate(m.begin(), m.end(), 0.0);
	REQUIRE(sum == 21.0);
}

TEST_CASE("Set column from vector and matrix", "[Matrix]") {
	matlib::Matrix m{3, 3, {1, 2, 3, 4, 5, 6, 7, 8, 9}};
	std::vector<double> newcol{10, 11, 12};
	m.set_column(newcol, 1);
	REQUIRE(m.column(1) == matlib::Matrix{3, 1, {10, 11, 12}});

	matlib::Matrix colmat{3, 1, {20, 21, 22}};
	m.set_column(colmat, 2);
	REQUIRE(m.column(2) == colmat);

	// invalid column indices
	REQUIRE_THROWS_AS(m.set_column(newcol, 3), std::invalid_argument);
	REQUIRE_THROWS_AS(m.set_column(newcol, -1), std::invalid_argument);

	// invalid sizes
	std::vector<double> shortvec{1, 2};
	REQUIRE_THROWS_AS(m.set_column(shortvec, 0), std::invalid_argument);

	matlib::Matrix badmat{2, 2, {1, 2, 3, 4}}; // not a nx1 column
	REQUIRE_THROWS_AS(m.set_column(badmat, 0), std::invalid_argument);
}

TEST_CASE("Submatrix extraction", "[Matrix]") {
	matlib::Matrix m{4, 4, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}};
	auto sub = m.submatrix(1, 1, 2, 3); // rows 1..2, cols 1..3
	REQUIRE(sub.rows() == 2);
	REQUIRE(sub.cols() == 3);
	REQUIRE(sub == matlib::Matrix{2, 3, {6, 7, 8, 10, 11, 12}});

	// out of bounds and negative indices
	REQUIRE_THROWS_AS(m.submatrix(-1, 0, 1, 1), std::invalid_argument);
	REQUIRE_THROWS_AS(m.submatrix(0, -1, 1, 1), std::invalid_argument);
	REQUIRE_THROWS_AS(m.submatrix(3, 0, 2, 1), std::invalid_argument); // row_start+rows > rows_
	REQUIRE_THROWS_AS(m.submatrix(0, 3, 1, 2), std::invalid_argument); // col_start+cols > cols_
}

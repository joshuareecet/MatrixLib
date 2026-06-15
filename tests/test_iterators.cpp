#include <matlib/matrix.hpp>
#include <catch2/catch_test_macros.hpp>
#include <algorithm>
#include <numeric>

TEST_CASE("Range-based-for sum equals manual indexed sum", "[iterators]") {
	matlib::Matrix m{2, 3, {1, 2, 3, 4, 5, 6}};

	// Sum via range-based for
	double rbf_sum = 0;
	for (auto val : m) {
		rbf_sum += val;
	}

	// Sum via manual indexing
	double indexed_sum = 0;
	for (int i = 0; i < m.rows(); ++i) {
		for (int j = 0; j < m.cols(); ++j) {
			indexed_sum += m(i, j);
		}
	}

	REQUIRE(rbf_sum == indexed_sum);
	REQUIRE(rbf_sum == 21.0);
}

TEST_CASE("std::accumulate over begin/end gives expected total", "[iterators]") {
	matlib::Matrix m{2, 2, {1.5, 2.5, 3.5, 4.5}};
	double sum = std::accumulate(m.begin(), m.end(), 0.0);
	REQUIRE(sum == 12.0);
}

TEST_CASE("std::max_element returns largest element", "[iterators]") {
	matlib::Matrix m{2, 3, {1, 7, 3, 4, 2, 6}};
	auto max_it = std::max_element(m.begin(), m.end());
	REQUIRE(*max_it == 7.0);
}

TEST_CASE("std::sort over begin/end sorts elements", "[iterators]") {
	matlib::Matrix m{2, 3, {5, 2, 8, 1, 9, 3}};
	// Note: std::sort will scramble the matrix structure; this test only proves iterators work
	std::sort(m.begin(), m.end());
	REQUIRE(std::is_sorted(m.begin(), m.end()));
}

TEST_CASE("Const iteration via range-based-for and cbegin/cend", "[iterators]") {
	const matlib::Matrix m{2, 2, {10, 20, 30, 40}};

	// Range-based for on const
	double rbf_sum = 0;
	for (auto val : m) {
		rbf_sum += val;
	}

	// cbegin/cend
	double cbegin_sum = std::accumulate(m.cbegin(), m.cend(), 0.0);

	REQUIRE(rbf_sum == cbegin_sum);
	REQUIRE(rbf_sum == 100.0);
}

TEST_CASE("Row iterators match operator()(i,j) and return correct stride", "[iterators]") {
	matlib::Matrix m{3, 4, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}};

	for (int i = 0; i < m.rows(); ++i) {
		double* row_start = m.row_begin(i);
		double* row_end = m.row_end(i);

		// Check stride equals cols
		REQUIRE(row_end - row_start == m.cols());

		// Check values match operator()
		for (int j = 0; j < m.cols(); ++j) {
			REQUIRE(row_start[j] == m(i, j));
		}
	}
}

TEST_CASE("Const row iterators return correct values", "[iterators]") {
	const matlib::Matrix m{2, 3, {1, 2, 3, 4, 5, 6}};

	for (int i = 0; i < m.rows(); ++i) {
		const double* row_start = m.row_begin(i);
		const double* row_end = m.row_end(i);

		REQUIRE(row_end - row_start == m.cols());
		for (int j = 0; j < m.cols(); ++j) {
			REQUIRE(row_start[j] == m(i, j));
		}
	}
}

TEST_CASE("row_begin out of bounds throws std::out_of_range", "[iterators]") {
	matlib::Matrix m{2, 2, {1, 2, 3, 4}};
	REQUIRE_THROWS_AS(m.row_begin(m.rows()), std::out_of_range);
	REQUIRE_THROWS_AS(m.row_begin(-1), std::out_of_range);
}

TEST_CASE("data() pointer matches operator()(0,0)", "[iterators]") {
	matlib::Matrix m{2, 2, {5, 6, 7, 8}};
	REQUIRE(m.data()[0] == m(0, 0));
	REQUIRE(m.data()[0] == 5.0);
}

TEST_CASE("const data() pointer works correctly", "[iterators]") {
	const matlib::Matrix m{2, 2, {10, 20, 30, 40}};
	REQUIRE(m.data()[0] == m(0, 0));
	REQUIRE(m.data()[1] == m(0, 1));
	REQUIRE(m.data()[2] == m(1, 0));
}

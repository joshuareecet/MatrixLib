#include <matlib/matrix.hpp>
#include <catch2/catch_test_macros.hpp>
#include "test_fixtures.hpp"

TEST_CASE("Matrix standard operations") {
	REQUIRE(test_fixtures::a == matlib::Matrix{2, 2, {1, 2, 3, 4}});
	REQUIRE(test_fixtures::a - test_fixtures::a == matlib::Matrix{2, 2, {0, 0, 0, 0}});
	REQUIRE(test_fixtures::a + matlib::Matrix{2, 2, {0, 0, 0, 0}} == test_fixtures::a);
	REQUIRE(test_fixtures::a + test_fixtures::diff == test_fixtures::b);
	REQUIRE(test_fixtures::b - test_fixtures::diff == test_fixtures::a);
	REQUIRE(test_fixtures::a * matlib::identity(test_fixtures::a.cols()) == test_fixtures::a);
	REQUIRE(2 * test_fixtures::a == matlib::Matrix(2, 2, {2, 4, 6, 8}));
	REQUIRE(test_fixtures::a * 2 == matlib::Matrix(2, 2, {2, 4, 6, 8}));
	REQUIRE(test_fixtures::a * test_fixtures::b == matlib::Matrix(2, 2, {19, 22, 43, 50}));
	REQUIRE((test_fixtures::a - test_fixtures::b).transpose() == (-1 * (test_fixtures::b - test_fixtures::a)).transpose());
}

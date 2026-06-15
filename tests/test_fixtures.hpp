#ifndef TEST_FIXTURES_HPP
#define TEST_FIXTURES_HPP

#include <matlib/matrix.hpp>

namespace test_fixtures {
	inline const matlib::Matrix a{2, 2, {1, 2, 3, 4}};
	inline const matlib::Matrix b{2, 2, {5, 6, 7, 8}};
	inline const matlib::Matrix diff{2, 2, {4, 4, 4, 4}};
	inline const matlib::Matrix non_square{2, 3, {1, 2, 3, 4, 5, 6}};
	inline const matlib::Matrix non_square_t{3, 2, {1, 4, 2, 5, 3, 6}};
}

#endif //TEST_FIXTURES_HPP

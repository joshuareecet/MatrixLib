#include <matlib/matrix.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <iostream>
#include <array>

TEST_CASE("Array determinant") {
	constexpr std::array<double, 4> m = {1.0, 2.0, 3.0, 4.0};
	REQUIRE(matlib::det2x2(m) == -2.0);
	static_assert(matlib::det2x2(m) == -2.0, "compile-time det2x2 failed");
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
	REQUIRE(move_time * 100 < copy_time);

	// verify moved-from state
	REQUIRE(big.rows() == 0);
	REQUIRE(big.cols() == 0);
	REQUIRE(big.size() == 0);

	// verify moved-to has the data
	REQUIRE(moved.rows() == size);
	REQUIRE(moved.cols() == size);
}

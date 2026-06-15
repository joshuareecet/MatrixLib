#include <matlib/matrix.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cmath>

TEST_CASE("Matrix norm operations") {
	using matlib::Norm;
	const matlib::Matrix norm_test{2, 2, {-1, 4, -5, -20}};
	double hand_calculated_frob_norm{std::sqrt((1.0 * 1) + (4.0 * 4) + (5.0 * 5) + (20.0 * 20))};
	double hand_calculated_L1_norm{24};
	double hand_calculated_Linf_norm{25};

	REQUIRE(matlib::approx_equal(2 * norm_test.norm(), (norm_test + norm_test).norm()));
	REQUIRE(matlib::approx_equal(norm_test.norm(Norm::Frobenius), hand_calculated_frob_norm));
	REQUIRE(matlib::approx_equal(norm_test.norm(Norm::L1), hand_calculated_L1_norm));
	REQUIRE(matlib::approx_equal(norm_test.norm(Norm::LInf), hand_calculated_Linf_norm));
}

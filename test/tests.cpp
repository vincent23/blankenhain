#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "AuxFunc.h"

TEST_CASE("Panning right works", "[aux]") {
	Sample input { 0.5 };
	aux::performPanning(input, 0.7f);
	alignas(16) double output[2];
	input.store_aligned(output);
	REQUIRE(output[0] == Approx(0.15000000596046448));
	REQUIRE(output[1] == Approx(0.64438813924789429));
}

TEST_CASE("Panning left works", "[aux]") {
	Sample input{ 0.5 };
	aux::performPanning(input, -0.7f);
	alignas(16) double output[2];
	input.store_aligned(output);
	REQUIRE(output[0] == Approx(0.64438813924789429));
	REQUIRE(output[1] == Approx(0.15000000596046448));
}

TEST_CASE("Panning center does not change anything", "[aux]") {
	Sample input{ 0.5 };
	aux::performPanning(input, 0.f);
	alignas(16) double output[2];
	input.store_aligned(output);
	REQUIRE(output[0] == Approx(0.5));
	REQUIRE(output[1] == Approx(0.5));
}
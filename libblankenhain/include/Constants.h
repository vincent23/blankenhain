#pragma once

#include <cmath>

namespace constants {
	constexpr unsigned int blockSize = 16;

	constexpr unsigned int parameterInterpolationLength = 256;

	const double pi = std::acos(-1.);

	const double sampleRate = 44100;
}
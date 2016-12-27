#pragma once

#include <cmath>

/**
 * Constants for the blankenhain2 environment
 * 
 * blockSize = bh2 processes incomming buffers in chunks. This integer values defines the maximum
 * chunk size that will be processed.

 **/
 namespace constants {
	constexpr unsigned int blockSize = 16;

	constexpr unsigned int parameterInterpolationLength = 256;

	const double pi = std::acos(-1.);

	const double sampleRate = 44100;
}
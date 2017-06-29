#pragma once
#include "Options.h"

template <typename T>
class CircularBuffer
{
public:
	// length must be power of two
	CircularBuffer(unsigned int length);
	~CircularBuffer();

	void push(const T& in) {
		unsigned int newPosition = (currentPosition + 1) & (length - 1);
		buffer[newPosition] = in;
		currentPosition = newPosition;
	}

	T get(unsigned int delaySamples) {
		// unsigned overflow will wrap around
		unsigned int tapPosition = (currentPosition - delaySamples) & (length - 1);
		return buffer[tapPosition];
	}

	T getInterpolated(float delaySamples) {
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
		if (delaySamples < 0.f) {
			throw "negative delay value";
		}
#endif
		unsigned int delaySamplesInteger = static_cast<unsigned int>(delaySamples);
		float fractionalPart = delaySamples - delaySamplesInteger; // probably not the most accurate way of doing this
		return T(1.f - fractionalPart) * get(delaySamples) + T(fractionalPart) * get(delaySamplesInteger + 1);
	}
	void reset();

private:
	T* const buffer;
	const unsigned int length;
	unsigned int currentPosition;
};
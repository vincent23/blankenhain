#pragma once
#include "Options.h"

template <typename T>
class CircularBuffer
{
public:
	// length must be power of two
	CircularBuffer(unsigned int length);
	~CircularBuffer();
	void push(const T& in);
	T get(unsigned int delaySamples);
	T getInterpolated(float delaySamples);
	void reset();

private:
	T* const buffer;
	const unsigned int length;
	unsigned int currentPosition;
};
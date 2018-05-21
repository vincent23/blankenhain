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

	T get(unsigned int delayToCurrentPosition_InSamples) const;

	T getInterpolated(float delayToCurrentPosition_InSamples) const;

	void reset();

	const unsigned int length;

private:
	T* const buffer;

	unsigned int currentPosition;
};
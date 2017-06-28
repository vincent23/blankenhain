#include "CircularBuffer.h"

// instance float circular buffer
template CircularBuffer<float>;

#include "Sample.h"
template CircularBuffer<Sample>;

template <typename T>
CircularBuffer<T>::CircularBuffer(unsigned int length_)
	: buffer(new T[length_])
	, length(length_)
	, currentPosition(0u)
{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	if (length == 0 || (length & (length - 1) != 0)) {
		throw "length is not power of two";
	}
#endif
	reset();
}

template <typename T>
CircularBuffer<T>::~CircularBuffer()
{
	delete[] buffer;
}

template <typename T>
void CircularBuffer<T>::push(const T& in)
{
	unsigned int newPosition = (currentPosition + 1) & (length - 1);
	buffer[newPosition] = in;
	currentPosition = newPosition;
}

template <typename T>
T CircularBuffer<T>::get(unsigned int delaySamples)
{
	// unsigned overflow will wrap around
	unsigned int tapPosition = (currentPosition - delaySamples) & (length - 1);
	return buffer[tapPosition];
}

template <typename T>
T CircularBuffer<T>::getInterpolated(float delaySamples)
{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	if (delaySamples < 0.f) {
		throw "negative delay value";
	}
#endif
	unsigned int delaySamplesInteger = static_cast<unsigned int>(delaySamples);
	float fractionalPart = delaySamples - delaySamplesInteger; // probably not the most accurate way of doing this
	return T(1.f - fractionalPart) * get(delaySamples) + T(fractionalPart) * get(delaySamplesInteger + 1);
}

template <typename T>
void CircularBuffer<T>::reset(void)
{
	for (unsigned int i = 0; i < length; i++)
	{
		buffer[i] = T();
	}
}
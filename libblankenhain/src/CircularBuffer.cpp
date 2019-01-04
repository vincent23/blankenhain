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
void CircularBuffer<T>::reset(void)
{
	for (unsigned int i = 0; i < length; i++)
	{
		buffer[i] = T();
	}
	currentPosition = 0u;
}

template <typename T>
void CircularBuffer<T>::push(const T& in) {
	const unsigned int newPosition = (currentPosition + 1) & (length - 1);
	buffer[newPosition] = in;
	currentPosition = newPosition;
}

template <typename T>
T CircularBuffer<T>::get(unsigned int delayToCurrentPosition_InSamples) const {
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	if (delayToCurrentPosition_InSamples < 0.f) {
		throw "negative delay value.";
	}
	if (delayToCurrentPosition_InSamples > length)
	{
		throw "requested delay bigger than buffer size.";
	}
#endif
	// unsigned overflow will wrap around
	const unsigned int tapPosition = (currentPosition - delayToCurrentPosition_InSamples) & (length - 1);
	return buffer[tapPosition];
}

template <typename T>
T CircularBuffer<T>::getInterpolated(float delayToCurrentPosition_InSamples) const {
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	if (delayToCurrentPosition_InSamples < 0.f) {
		throw "negative delay value";
	}
	if (delayToCurrentPosition_InSamples > length)
	{
		throw "requested delay bigger than buffer size.";
	}
#endif
	const unsigned int delaySamplesInteger = static_cast<unsigned int>(delayToCurrentPosition_InSamples);
	const float fractionalPart = delayToCurrentPosition_InSamples - static_cast<float>(delaySamplesInteger); // probably not the most accurate way of doing this
	return T(1.f - fractionalPart) * get(delaySamplesInteger) + T(fractionalPart) * get(delaySamplesInteger + 1);
}
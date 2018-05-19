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
}
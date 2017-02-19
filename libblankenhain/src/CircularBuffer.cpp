#include "CircularBuffer.h"

// instance float circular buffer
//template CircularBuffer<float>;

#include "Sample.h"
template CircularBuffer<Sample>;
template LinearInterpolatedCircularBuffer<Sample>;

template <typename T>
CircularBuffer<T>::CircularBuffer(size_t numberOfSamples_)
	: buffer(nullptr), numberOfSamples(numberOfSamples_),
	maxNumberOfSamples(numberOfSamples), oldNumberOfSamples(0u),
	currentPosition(0u)
{
	buffer = new T[maxNumberOfSamples];
	for (size_t i = 0; i < maxNumberOfSamples; i++)
	{
		buffer[i] = T();
	}
};

template <typename T>
CircularBuffer<T>::~CircularBuffer(void)
{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	if (this->buffer == nullptr)
		throw ("buffer is already nullptr\n");
#endif
	delete[] buffer;
	buffer = nullptr;
}

template <typename T>
size_t CircularBuffer<T>::getCurrentIteratorInDelayline() const
{
	return this->currentPosition;
}

template <typename T>
void _vectorcall CircularBuffer<T>::push(T in)
{
	buffer[currentPosition] = in;
	currentPosition++;
	if (currentPosition == oldNumberOfSamples && oldNumberOfSamples != 0u)
	{
		currentPosition = 0u;
		oldNumberOfSamples = 0u;
	}
	else if (currentPosition == numberOfSamples) currentPosition = 0u;
}

template <typename T>
T _vectorcall CircularBuffer<T>::pushpop(T in)
{
	T out = buffer[currentPosition];
	buffer[currentPosition] = in;
	currentPosition++;
	if (currentPosition == oldNumberOfSamples && oldNumberOfSamples != 0u)
	{
		currentPosition = 0u;
		oldNumberOfSamples = 0u;
	}
	else if (currentPosition == numberOfSamples) currentPosition = 0u;
	return out;
}

template <typename T>
size_t CircularBuffer<T>::getSize() const
{
	return numberOfSamples;
}

template <typename T>
void CircularBuffer<T>::setSize(size_t size_)
{
	if (size_ == numberOfSamples) return;
	else if (size_ >= maxNumberOfSamples)
	{
		T* temp = new T[size_];
		for (size_t i = 0; i < numberOfSamples; i++)
		{
			temp[i] = buffer[i];
		}
		for (size_t i = numberOfSamples; i < size_; i++)
		{
			temp[i] = T();
		}
		delete[] buffer;
		buffer = temp;
		temp = nullptr;
		numberOfSamples = size_;
		maxNumberOfSamples = size_;
	}
	else if (size_ > numberOfSamples)
	{
		oldNumberOfSamples = numberOfSamples;
		numberOfSamples = size_;
	}
	else
	{
		numberOfSamples = size_;
	}
	currentPosition = 0u;
};

template <typename T>
T _vectorcall CircularBuffer<T>::get(int iterator)
{
	// returning directly produces an internal compiler error right now
	T returnValue;
	if (iterator == -1) returnValue = buffer[currentPosition];
#ifndef _LIBBLANKENHAIN_ENABLE_WARNINGS
	else
	{
		returnValue = buffer[iterator];
	}
#else
	else if (iterator < static_cast<int>(numberOfSamples))
	{
		returnValue = buffer[iterator];
	}
	else
		throw ("circular buffer access out of bounds\n");
#endif
	return returnValue;
};

template <typename T>
void CircularBuffer<T>::reset(void)
{
	for (unsigned int i = 0u; i < this->numberOfSamples; i++)
	{
		buffer[i] = T();
	}
}

////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
T _vectorcall LinearInterpolatedCircularBuffer<T>::interpolate(T valueBegin, T valueEnd, float ratio)
{
	return valueBegin + (valueEnd - valueBegin) * T(ratio);
}

template <typename T>
void LinearInterpolatedCircularBuffer<T>::setSize(size_t size_)
{
	if (size_ == numberOfSamples) return;
	currentPosition = static_cast<size_t>(static_cast<float>(currentPosition) * static_cast<float>(size_) / static_cast<float>(numberOfSamples));
	if (size_ > numberOfSamples)
	{
		T* temp = new T[size_];

		for (size_t i = 0; i < size_; i++)
		{
			float multiplier = static_cast<float>(i) * (static_cast<float>(numberOfSamples) / static_cast<float>(size_));
			size_t baseValueMatch = static_cast<size_t>(multiplier);
			float ratio = multiplier - static_cast<float>(baseValueMatch);
			temp[i] = this->interpolate(buffer[baseValueMatch], buffer[baseValueMatch + 1], ratio);
		}
		
		delete[] buffer;
		buffer = temp;
		temp = nullptr;
		numberOfSamples = size_;
		if(maxNumberOfSamples < size_)
			maxNumberOfSamples = size_;
	}
	else
	{
		for (size_t i = 0; i < size_; i++)
		{
			float multiplier = static_cast<float>(i) * (static_cast<float>(numberOfSamples) / static_cast<float>(size_));
			size_t baseValueMatch = static_cast<size_t>(multiplier);
			float inBetweenStepsRatio = multiplier - static_cast<float>(baseValueMatch);
			buffer[i] = this->interpolate(buffer[baseValueMatch], buffer[baseValueMatch + 1], inBetweenStepsRatio);
		}

		numberOfSamples = size_;
	}
}

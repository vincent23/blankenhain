#include "CircularBuffer.h"
// instance float circular buffer
//template CircularBuffer<float>;

#include "Sample.h"
template CircularBuffer<Sample>;
template OnePoleFilter<Sample>;

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
	if (this->buffer != nullptr)
	{
		delete[] buffer;
		buffer = nullptr;
	}
}

template <typename T>
void CircularBuffer<T>::push(T const& in)
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
T CircularBuffer<T>::pushpop(T const& in)
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
size_t CircularBuffer<T>::getSize()
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
T CircularBuffer<T>::get(int iterator)
{
	if (iterator == -1) return buffer[currentPosition];
	else if (iterator < static_cast<int>(numberOfSamples))
	{
		return buffer[iterator];
	}
	else
	{
		return T();
	}
};




template <typename T>
OnePoleFilter<T>::OnePoleFilter<T>(T const& polePosition)
{
  gain = Sample(1.);
  this->setPole(polePosition);
}

template <typename T>
OnePoleFilter<T>::~OnePoleFilter<T>(void) {};


template <typename T>
T OnePoleFilter<T>::tick(T const& in)
{
  inputValue = gain * in;
  Sample lastFrame_ = param_b0 * inputValue - param_a1 * outputValue0;
  outputValue0 = lastFrame_;

  return lastFrame_;
}

template <typename T>
void OnePoleFilter<T>::setGain(T const& gain_)
{
  gain = gain_;
}

template <typename T>
T OnePoleFilter<T>::getGain() const
{
  return gain;
}

template <typename T>
void OnePoleFilter<T>::setParams(T const& b0, T const& a1)
{
  //if (std::abs(a1) >= 1.0) {
  //  oStream_ << "OnePole::setCoefficients: a1 argument (" << a1 << ") should be less than 1.0!";
  //  handleError(StkError::WARNING); return;
  //}
  this->param_b0 = b0;
  this->param_a1 = a1;
  //if (clearState) this->clear();
}

template <typename T>
void OnePoleFilter<T>::getParams(T & b0, T& a1) const
{
  b0 = param_b0;
  a1 = param_a1;
}

template <typename T>
void OnePoleFilter<T>::setPole(T const& polePosition)
{
  //(if (std::abs(thePole) >= 1.0) {
  //(  oStream_ << "OnePole::setPole: argument (" << thePole << ") should be less than 1.0!";
  //(  handleError(StkError::WARNING); return;
  //(}

  // Normalize coefficients for peak unity gain.
  //if (polePosition > T(0.0))
    param_b0 = (T(1.0) - polePosition);
  //else
    //param_b0 = (T(1.0) + polePosition);

  param_a1 = T(-1.) * polePosition;
}
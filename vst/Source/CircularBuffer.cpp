/*
  ==============================================================================

    CircularBuffer.cpp
    Created: 26 Mar 2016 2:38:20am
    Author:  Dustin Kaiser

  ==============================================================================
*/
#include "CircularBuffer.h"
#ifdef CIRCULARBUFFER_H_INCLUDED

template <typename T>
CircularBuffer<T>::CircularBuffer(size_t numberOfSamples_) : buffer(nullptr), numberOfSamples(numberOfSamples_)
{
  buffer = new T[numberOfSamples];
  for (size_t i = 0; i < numberOfSamples; i++)
  {
    buffer[i] = T();
  }
  for (size_t i = 0; i < 2; i++) minMaxStore[i] = &buffer[0];
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
  for (size_t i = 0; i < numberOfSamples - 1u; i++)
  {
    buffer[i + 1u] = buffer[i];
  }
  buffer[0] = in;
  for (size_t i = 0; i < 4; i++)
  {
    minMaxStore[i]++;
    if (minMaxStore[i] > &buffer[numberOfSamples - 1u])
    {
      if (i == 0) this->searchMin();
      else if (i == 1) this->searchMax();
      else if (i == 2) this->searchAbsMin();
      else if (i == 3) this->searchAbsMax();
    }
  }
}

template <typename T>
T CircularBuffer<T>::pushpop(T const& in)
{
  T holder = buffer[numberOfSamples - 1u];
  for (size_t i = 0; i < numberOfSamples - 1u; i++)
  {
    buffer[i + 1u] = buffer[i];
  }
  buffer[0] = in;

  for (size_t i = 0; i < 4; i++)
  {
    minMaxStore[i]++;
    if (minMaxStore[i] > &buffer[numberOfSamples - 1u])
    {
      if (i == 0u) this->searchMin();
      else if (i == 1u) this->searchMax();
      else if (i == 2u) this->searchAbsMin();
      else if (i == 3u) this->searchAbsMax();
    }
  }

  return holder;
}

template <typename T>
size_t CircularBuffer<T>::getSize()
{
  return numberOfSamples;
}

template <typename T>
void CircularBuffer<T>::setSize(size_t size_)
{
  if (size_ < numberOfSamples)
  {
    T* temp = new T[size_];
    for (size_t i = 0; i < size_; i++)
    {
      temp[i] = buffer[i];
    }
    delete[] buffer;
    buffer = temp;
    temp = nullptr;
  }
  else
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
  }
  this->numberOfSamples = size_;
  this->searchMin();
  this->searchMax();
  this->searchAbsMin();
  this->searchAbsMax();
};

template <typename T>
T CircularBuffer<T>::get(size_t iterator)
{
  if (iterator < numberOfSamples)
  {
    return buffer[iterator];
  }
  else
  {
    return T();
  }
};

template <typename T>
void CircularBuffer<T>::searchMin()
{
  T* minimum = &buffer[0];
  for (size_t i = 1; i < numberOfSamples; i++)
  {
    if (buffer[i] < *minimum)
    {
      minimum = &buffer[i];
    }
  }
  minMaxStore[0] = minimum;
}

template <typename T>
void CircularBuffer<T>::searchMax()
{
  T* maximum = &buffer[0];
  for (size_t i = 1; i < numberOfSamples; i++)
  {
    if (buffer[i] > *maximum)
    {
      maximum = &buffer[i];
    }
  }
  minMaxStore[1] = maximum;
}

template <typename T>
void CircularBuffer<T>::searchAbsMax()
{
  T maximum = buffer[0] > 0.f ? buffer[0] : -1.f * buffer[0];
  T* ptr = &buffer[0];
  for (size_t i = 1; i < numberOfSamples; i++)
  {
    T temp = buffer[i] > 0.f ? buffer[i] : -1.f * buffer[i];
    if (temp > maximum)
    {
      ptr = &buffer[i];
      maximum = temp;
    }
  }
  minMaxStore[3] = ptr;
}

template <typename T>
void CircularBuffer<T>::searchAbsMin()
{
  T min = buffer[0] > 0.f ? buffer[0] : -1.f * buffer[0];
  T* ptr = &buffer[0];
  for (size_t i = 1; i < numberOfSamples; i++)
  {
    T temp = buffer[i] > 0.f ? buffer[i] : -1.f * buffer[i];
    if (temp < min)
    {
      ptr = &buffer[i];
      min = temp;
    }
  }
  minMaxStore[2] = ptr;
}

template <typename T>
T CircularBuffer<T>::getMin()
{
  return *minMaxStore[0];
}

template <typename T>
T CircularBuffer<T>::getMax()
{
  return *minMaxStore[1];
}

template <typename T>
T CircularBuffer<T>::getAbsMin()
{
  return (*minMaxStore[2] > 0.f ? *minMaxStore[2] : -1.f * (*minMaxStore[2]));
}

template <typename T>
T CircularBuffer<T>::getAbsMax()
{
  return (*minMaxStore[3] > 0.f ? *minMaxStore[3] : -1.f * (*minMaxStore[3]));
}

template <typename T>
T CircularBuffer<T>::getSum()
{
  T summed = T();
  for (size_t i = 0; i < numberOfSamples; i++) summed += buffer[i];
  return summed;
}


// instance float circular buffer
template CircularBuffer<float>;
#endif
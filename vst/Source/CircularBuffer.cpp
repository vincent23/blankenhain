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
  for (int i = 0; i < numberOfSamples; i++)
  {
    buffer[i] = T();
  }
};

template <typename T>
CircularBuffer<T>::~CircularBuffer(void)
{
  delete[] buffer;
}

template <typename T>
void CircularBuffer<T>::push(T const& in)
{
  for (int i = 0; i < numberOfSamples - 1u; i++)
  {
    buffer[i + 1u] = buffer[i];
  }
  buffer[0] = in;
}

template <typename T>
T CircularBuffer<T>::pushpop(T const& in)
{
  T holder = buffer[numberOfSamples - 1u];
  for (int i = 0; i < numberOfSamples - 1u; i++)
  {
    buffer[i + 1u] = buffer[i];
  }
  buffer[0] = in;
  return holder;
}

template <typename T>
T CircularBuffer<T>::pop()
{
  return buffer[numberOfSamples - 1u];
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
    for (int i = 0; i < size_; i++)
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
    for (int i = 0; i < numberOfSamples; i++)
    {
      temp[i] = buffer[i];
    }
    for (int i = numberOfSamples; i < size_; i++)
    {
      temp[i] = T();
    }
    delete[] buffer;
    buffer = temp;
    temp = nullptr;
  }
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
T CircularBuffer<T>::getMin()
{
  T* minimum == &buffer[0];
  for (int i = 1; i < numberOfSamples; i++)
  {
    if (buffer[i] < *minimum)
    {
      minimum = &buffer[i];
    }
  }
  return *minimum;
}

template <typename T>
T CircularBuffer<T>::getMax()
{
  T* maximum = &buffer[0];
  for (int i = 1; i < numberOfSamples; i++)
  {
    if (buffer[i] > *maximum)
    {
      maximum = &buffer[i];
    }
  }
  return *maximum;
}

template <typename T>
T CircularBuffer<T>::getSum()
{
  T summed;
  for (int i = 0; i < numberOfSamples; i++) summed += buffer[i];
  return summed;
}
#endif
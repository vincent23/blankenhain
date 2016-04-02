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
CircularBuffer<T>::CircularBuffer(size_t numberOfSamples_) 
  : buffer(nullptr), numberOfSamples(numberOfSamples_),
  maxNumberOfSamples(numberOfSamples), oldNumberOfSamples(0u),
  currentPosition(0u)
{
  buffer = new T[maxNumberOfSamples];
  for (size_t i = 0; i < numberOfSamples; i++)
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
  if(size_ >= maxNumberOfSamples)
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
  }
  else if (size_ > numberOfSamples)
  {
    oldNumberOfSamples = numberOfSamples;
    numberOfSamples = size_;
  }
  else if (size_ == numberOfSamples) return;
  else
  {
    numberOfSamples = size_;
  }
  currentPosition = 0u;
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

// instance float circular buffer
template CircularBuffer<float>;
#endif
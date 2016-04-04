/*
  ==============================================================================

    CircularBuffer.h
    Created: 26 Mar 2016 2:38:20am
    Author:  Dustin Kaiser

  ==============================================================================
*/

#ifndef CIRCULARBUFFER_H_INCLUDED
#define CIRCULARBUFFER_H_INCLUDED

#pragma once
#include <juce>

template <typename T>
class CircularBuffer
{
protected:
  T* buffer;
  size_t numberOfSamples;
  size_t maxNumberOfSamples;
  size_t oldNumberOfSamples;
  size_t currentPosition;
public:
  CircularBuffer(size_t numberOfSamples);
  ~CircularBuffer(void);
  void push(T const& in);
  T pushpop(T const& in);
  size_t getSize();
  void setSize(size_t size_);
  T get(int iterator = -1);
};

#endif  // CIRCULARBUFFER_H_INCLUDED

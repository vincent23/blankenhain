/*
  ==============================================================================

    CircularBuffer.h
    Created: 26 Mar 2016 2:38:20am
    Author:  Dustin Kaiser

  ==============================================================================
*/

#ifndef CIRCULARBUFFER_H_INCLUDED
#define CIRCULARBUFFER_H_INCLUDED

template <typename T>
class CircularBuffer
{
protected:
  T* buffer;
  size_t numberOfSamples;
  size_t currentPosition;
public:
  CircularBuffer(size_t numberOfSamples);
  ~CircularBuffer(void);
  void push(T const& in);
  T pushpop(T const& in);
  size_t getSize();
  void setSize(size_t size_);
  T get(size_t iterator);
};

template <typename T>
class AdvancedCircularBuffer
{
protected:
  T* buffer;
  size_t numberOfSamples;
  T* minMaxStore[4]; // min, max, absmin, absmax
public:
  AdvancedCircularBuffer(size_t numberOfSamples);
  ~AdvancedCircularBuffer(void);
  void push(T const& in);
  T pushpop(T const& in);
  size_t getSize();
  void setSize(size_t size_);
  T get(size_t iterator);
  T getMin();
  T getAbsMax();
  T getAbsMin();
  T getMax();
  void searchMin();
  void searchMax();
  void searchAbsMin();
  void searchAbsMax();
  T getSum();
};


#endif  // CIRCULARBUFFER_H_INCLUDED

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
private:
  T* buffer;
  size_t numberOfSamples;
public:
  CircularBuffer(size_t numberOfSamples);
  ~CircularBuffer(void);
  void push(T const& in);
  T pop();
  T pushpop(T const& in);
  size_t getSize();
  void setSize(size_t size_);
  T get(size_t iterator);
  T getMin();
  T getMax();
  T getSum();
};



#endif  // CIRCULARBUFFER_H_INCLUDED

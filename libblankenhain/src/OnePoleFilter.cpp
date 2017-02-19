// instance float circular buffer
#include "Sample.h"
#include "OnePoleFilter.h"
template OnePoleFilter<Sample>;

template <typename T>
OnePoleFilter<T>::OnePoleFilter(const T& polePosition)
{
  gain = T(1.);
  this->setPole(polePosition);
}

template <typename T>
OnePoleFilter<T>::~OnePoleFilter(void) {};


template <typename T>
T _vectorcall OnePoleFilter<T>::tick(T in)
{
  inputValue = gain * in;
  T lastFrame_ = param_b0 * inputValue - param_a1 * outputValue0;
  outputValue0 = lastFrame_;

  return outputValue0;
}

template <typename T>
void _vectorcall OnePoleFilter<T>::setGain(T gain_)
{
  gain = gain_;
}

template <typename T>
T _vectorcall OnePoleFilter<T>::getGain() const
{
  return gain;
}

template <typename T>
void _vectorcall OnePoleFilter<T>::setParams(T b0, T a1)
{
  this->param_b0 = b0;
  this->param_a1 = a1;
}

template <typename T>
void _vectorcall OnePoleFilter<T>::getParams(T& b0, T& a1) const
{
  b0 = param_b0;
  a1 = param_a1;
}

template <typename T>
void _vectorcall OnePoleFilter<T>::setPole(T polePosition)
{
  param_b0 = (T(1.0) - polePosition);
  param_a1 = T(-1.) * polePosition;
}
// instance float circular buffer
#include "Sample.h"
#include "OnePoleFilter.h"
template OnePoleFilter<Sample>;

template <typename T>
OnePoleFilter<T>::OnePoleFilter(T const& polePosition)
{
  gain = Sample(1.);
  this->setPole(polePosition);
}

template <typename T>
OnePoleFilter<T>::~OnePoleFilter(void) {};


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
  this->param_b0 = b0;
  this->param_a1 = a1;
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
  param_b0 = (T(1.0) - polePosition);
  param_a1 = T(-1.) * polePosition;
}
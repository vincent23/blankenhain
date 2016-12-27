#pragma once
// adapted from stk freeverb
// https://github.com/thestk/stk/blob/master/src/FreeVerb.cpp
// https://github.com/thestk/stk/blob/master/include/FreeVerb.h
//
// Mainly used in reverb

template <typename T>
class OnePoleFilter : public AlignedType
{
protected:
  T param_b0;
  T param_a1;
  T gain;
  T inputValue, outputValue0;
public:
  OnePoleFilter(T const& polePosition);
  ~OnePoleFilter(void);
  T tick(T const& in);
  void setParams(T const& b0, T const&a1);
  void setGain(T const& gain);
  T getGain() const;
  void getParams(T & b0, T& a1) const;
  void setPole(T const& polePosition);
};
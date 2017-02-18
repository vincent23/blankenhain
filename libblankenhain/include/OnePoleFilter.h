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
  OnePoleFilter(const T& polePosition);
  ~OnePoleFilter(void);
  T _vectorcall tick(const T& in);
  void _vectorcall setParams(const T& b0, const T& a1);
  void _vectorcall setGain(T const& gain);
  T _vectorcall getGain() const;
  void _vectorcall getParams(T& b0, T& a1) const;
  void _vectorcall setPole(const T& polePosition);
};
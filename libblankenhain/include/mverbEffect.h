#pragma once

#include "EffectBase.h"
#include "CircularBuffer.h"

// adapted from stk freeverb
// https://github.com/thestk/stk/blob/master/src/FreeVerb.cpp
// https://github.com/thestk/stk/blob/master/include/FreeVerb.h
class mverbEffect : public EffectBase
{
public:
	// Change the name and define parameters in constructor
  mverbEffect();
  ~mverbEffect(void);
	void process(Sample* buffer, size_t numberOfSamples) override;

protected:


	// Include optional additional stuff
	
  const int nCombs = 8;
  const int nAllpasses = 4;
  const int stereoSpread = 23;
  // Delay line lengths for 44100Hz sampling rate.
  unsigned int cDelayLengths[8] = { 1617, 1557, 1491, 1422, 1356, 1277, 1188, 1116 };
  unsigned int aDelayLengths[4] = { 225, 556, 441, 341 };

  // Set static delay line lengths
  const float fixedGain = 0.015;
  const float scaleDamp = 0.4;
  const float scaleRoom = 0.28;
  const float offsetRoom = 0.7;



  const float g_ = 0.5; // allpass coefficient, immutable in FreeVerb

  Sample lastFrame_;

  // LBFC: Lowpass Feedback Comb Filters
  CircularBuffer<Sample>** combDelay_;

  OnePoleFilter<Sample>** combLP_;

  // AP: Allpass Filters
  CircularBuffer<Sample>** allPassDelay_;

};

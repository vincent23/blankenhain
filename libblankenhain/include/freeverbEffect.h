#pragma once
#include "ParameterBundle.h"
#include "AuxFunc.h"
#include "InterpolatedValue.h"
#include "OnePoleFilter.h"
#include "EffectBase.h"
#include "CircularBuffer.h"

// adapted from stk freeverb
// https://github.com/thestk/stk/blob/master/src/FreeVerb.cpp
// https://github.com/thestk/stk/blob/master/include/FreeVerb.h
class freeverbEffect : public EffectBase
{
public:
	// Change the name and define parameters in constructor
	freeverbEffect();
	~freeverbEffect(void);
	void process(Sample* buffer, size_t numberOfSamples) override;

protected:
	// Include optional additional stuff

	const unsigned int nCombs = 8;
	const unsigned int nAllpasses = 4;
	const unsigned int stereoSpread = 23;
	// Delay line lengths for 44100Hz sampling rate.
	unsigned int cDelayLengths[8] = { 1617, 1557, 1491, 1422, 1356, 1277, 1188, 1116 };
	unsigned int aDelayLengths[4] = { 225, 556, 441, 341 };

	// Set static delay line lengths
	const float fixedGain = 0.015f;
	const float scaleDamp = 0.4f;
	const float scaleRoom = 0.28f;
	const float offsetRoom = 0.7f;

	const Sample g_ = Sample(0.5f); // allpass coefficient, immutable in FreeVerb

	// LBFC: Lowpass Feedback Comb Filters
	CircularBuffer<Sample>* combDelay_[8];
	CircularBuffer<Sample>* combDelayRight[8];

	OnePoleFilter<Sample>* combLP_[8];

	// AP: Allpass Filters
	CircularBuffer<Sample>* allPassDelay_[4];
	CircularBuffer<Sample>* allPassDelayRight[4];

};

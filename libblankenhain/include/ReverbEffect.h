/*
 * The algorithm is taken and modified from STK::freeverb.
 * The STK Licence (https://ccrma.stanford.edu/software/stk/faq.html)
 * requires us to display the following copyright statement in full.
 *
 * The Synthesis ToolKit in C++ (STK) is a set of open source audio
 * signal processing and algorithmic synthesis classes written in the
 * C++ programming language. STK was designed to facilitate rapid
 * development of music synthesis and audio processing software, with
 * an emphasis on cross-platform functionality, realtime control,
 * ease of use, and educational example code.  STK currently runs
 * with realtime support (audio and MIDI) on Linux, Macintosh OS X,
 * and Windows computer platforms. Generic, non-realtime support has
 * been tested under NeXTStep, Sun, and other platforms and should
 * work with any standard C++ compiler.
 *
 * STK WWW site: http://ccrma.stanford.edu/software/stk/
 *
 * The Synthesis ToolKit in C++ (STK)
 * Copyright (c) 1995--2016 Perry R. Cook and Gary P. Scavone
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * Any person wishing to distribute modifications to the Software is
 * asked to send the modifications to the original developer so that they
 * can be incorporated into the canonical version.  This is, however, not
 * a binding provision of this license.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#pragma once

#include "ParameterBundle.h"
#include "AuxFunc.h"
#include "InterpolatedValue.h"
#include "OnePoleFilter.h"
#include "EffectBase.h"
#include "CircularBuffer.h"

class ReverbEffect : public EffectBase
{
public:
	// Change the name and define parameters in constructor
	ReverbEffect();
	void process(Sample* buffer, size_t numberOfSamples, size_t currentTime) override;
	void resetEffect();

protected:
	// Include optional additional stuff

	const unsigned int nCombs = 8;
	const unsigned int nAllpasses = 4;
	const unsigned int stereoSpread = 23;
	// Delay line lengths for 44100Hz sampling rate.
	const unsigned int cDelayLengths[8] = { 1617, 1557, 1491, 1422, 1356, 1277, 1188, 1116 };
	const unsigned int aDelayLengths[4] = { 225, 556, 441, 341 };

	// Set static delay line lengths
	const float fixedGain = 0.015f;
	const float scaleDamp = 0.4f;
	const float scaleRoom = 0.28f;
	const float offsetRoom = 0.7f;

	const Sample g_ = Sample(0.5f); // allpass coefficient, immutable in FreeVerb

	// LBFC: Lowpass Feedback Comb Filters
	CircularBuffer<float> combDelay_l[8];
	CircularBuffer<float> combDelay_r[8];

	OnePoleFilter<Sample> combLP_[8];

	// AP: Allpass Filters
	CircularBuffer<float> allPassDelay_l[4];
	CircularBuffer<float> allPassDelay_r[4];

private:
	static OnePoleFilter<Sample> getCombLP();
	static CircularBuffer<float> getCombDelay();
	static CircularBuffer<float> getAllPassDelay();
};

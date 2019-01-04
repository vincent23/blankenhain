#pragma once
#include "freeverbEffect.h"

// TODO improve initialization (maybe some template magic?)
freeverbEffect::freeverbEffect() : EffectBase(4u)
, combLP_{ getCombLP(),getCombLP(),getCombLP(),getCombLP(),getCombLP(),getCombLP(),getCombLP(),getCombLP() }
, combDelay_l{ getCombDelay(),getCombDelay(),getCombDelay(),getCombDelay(),getCombDelay(),getCombDelay(),getCombDelay(),getCombDelay() }
, combDelay_r{ getCombDelay(),getCombDelay(),getCombDelay(),getCombDelay(),getCombDelay(),getCombDelay(),getCombDelay(),getCombDelay() }
, allPassDelay_l{ getAllPassDelay(),getAllPassDelay(),getAllPassDelay(),getAllPassDelay() }
, allPassDelay_r{ getAllPassDelay(),getAllPassDelay(),getAllPassDelay(),getAllPassDelay() }
{
	ParameterBundle& params = getParameterBundle();

	// Insert your stuff here
	params.initParameter(0, new FloatParameter(0.75f, NormalizedRange(), "roomSize", ""));
	params.initParameter(1, new FloatParameter(0.25f, NormalizedRange(), "damping", ""));
	params.initParameter(2, new FloatParameter(0.f, NormalizedRange(), "width", ""));
	params.initParameter(3, new FloatParameter(0., NormalizedRange(0.f,100.f, 0.5f), "DryWet", ""));
}

OnePoleFilter<Sample> freeverbEffect::getCombLP()
{
	return OnePoleFilter<Sample>(Sample(0.5));
}

CircularBuffer<float> freeverbEffect::getCombDelay()
{
	return CircularBuffer<float>(2048u);
}

CircularBuffer<float> freeverbEffect::getAllPassDelay()
{
	return CircularBuffer<float>(1024u);
}

void freeverbEffect::resetEffect()
{
	for (size_t i = 0u; static_cast<int>(i) < this->nCombs; i++)
	{
		combDelay_l[i].reset();
		combDelay_r[i].reset();
	}
	for (size_t i = 0u; i < static_cast<size_t>(this->nAllpasses); i++)
	{
		allPassDelay_l[i].reset();
		allPassDelay_r[i].reset();
	}
}

/*
 * The algorithm is taken and modified from STK::freeverb.
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
void freeverbEffect::process(Sample* buffer, size_t numberOfSamples, size_t currentTime)
{
	const float roomSize = interpolatedParameters.get(0);
	const float width = interpolatedParameters.get(2);

	const float damping = interpolatedParameters.get(1) * scaleDamp;

	const Sample currentRoomSize = Sample((roomSize * scaleRoom) + offsetRoom);
	InterpolatedValue<float> const& drywet = interpolatedParameters.get(3);

	// set low pass filter for delay output
	for (unsigned int i = 0; i < nCombs; i++)
	{
		combLP_[i].setParams(Sample(1.0 - damping), Sample(-1.0 * damping));
	}

	const float widthConst = width;
	const float drywetBefore = drywet.get() / 100.f;
	const float drywetAfter = drywet.get(numberOfSamples) / 100.f;
	InterpolatedValue<float> drywetInterpol(drywetBefore, drywetAfter, numberOfSamples);

	Sample input[constants::blockSize];
	for (unsigned int i = 0; i < numberOfSamples; i++) {
		input[i] = buffer[i];
		buffer[i] = Sample(0);
	}

	// Parallel LBCF filters
	alignas(16) double lr[2];
	for (unsigned int j = 0; j < nCombs; j++)
	{
		for (size_t i = 0; i < numberOfSamples; i++)
		{
			// Left channel
			const float delayedLeft = combDelay_l[j].get(cDelayLengths[j]);
			const float delayedRight = combDelay_r[j].get(cDelayLengths[j] + 23);
			const Sample delayed(delayedLeft, delayedRight);
			Sample yn = input[i] + (currentRoomSize * combLP_[j].tick(delayed));
			yn.store_aligned(lr);
			combDelay_l[j].push(static_cast<float>(lr[0]));
			combDelay_r[j].push(static_cast<float>(lr[1]));
			buffer[i] += yn;
		}
	}

	// Series allpass filters
	for (unsigned int j = 0; j < nAllpasses; j++)
	{
		for (size_t i = 0; i < numberOfSamples; i++)
		{
			// Left channel
			const float vn_m_left = allPassDelay_l[j].get(aDelayLengths[j]);
			const float vn_m_right = allPassDelay_r[j].get(aDelayLengths[j] + 23);
			const Sample vn_m(vn_m_left, vn_m_right);
			Sample vn = buffer[i] + g_ * vn_m;
			vn.store_aligned(lr);
			allPassDelay_l[j].push(static_cast<float>(lr[0]));
			allPassDelay_r[j].push(static_cast<float>(lr[1]));

			// calculate output
			buffer[i] = vn_m - buffer[i];
		}
	}



	for (size_t i = 0; i < numberOfSamples; i++)
	{
		const float drywetCur = drywetInterpol.get(i);
		const float wetIn = drywetCur >= 0.5f ? 1.f : drywetCur * 2.f;
		const Sample wet1(wetIn * (widthConst * .5f + .5f));
		const Sample wet2(wetIn * (.5f - widthConst * .5f));
		// Mix output
		// Correct way too loud wet volume
		buffer[i] /= Sample((nCombs+nAllpasses) * 2.f);
		// And some more headspace via magic number....
		buffer[i] *= Sample(aux::decibelToLinear(-3.f));

		buffer[i] = wet1 * buffer[i] + wet2 * buffer[i].flippedChannels();
		buffer[i] += input[i] * Sample(drywetCur >= 0.5f ? (1.0f - drywetCur) * 2.f : 1.f);
	}
}
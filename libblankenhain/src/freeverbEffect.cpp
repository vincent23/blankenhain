#pragma once
#include "freeverbEffect.h"

// Here, enter the number of GUI parameters you want to have
const unsigned int NUMBER_OF_PARAMETERS = 6u;

// Change the name and define parameters in constructor
freeverbEffect::freeverbEffect() : EffectBase(NUMBER_OF_PARAMETERS)
{
	ParameterBundle* params = getPointerToParameterBundle();


	// Insert your stuff here
	(params->getParameter(0)) = new FloatParameter(0.75f, NormalizedRange(), "roomSize", "");
	(params->getParameter(1)) = new FloatParameter(0.25f, NormalizedRange(), "damping", "");
	(params->getParameter(2)) = new FloatParameter(0.f, NormalizedRange(), "width", "");
	(params->getParameter(3)) = new FloatParameter(0.f, NormalizedRange(true), "mode", "bool");
	(params->getParameter(4)) = new FloatParameter(-6.f, NormalizedRange(-120.f, 0.f, 5.f), "dry", "dB");
	(params->getParameter(5)) = new FloatParameter(-60.f, NormalizedRange(-120.f, 0.f, 5.f), "wet", "dB");

	///////////////////////
	// More stuff //

	for (size_t i = 0u; static_cast<int>(i) < this->nCombs; i++)
	{
		combDelay_[i] = new CircularBuffer<Sample>(cDelayLengths[i]);
		combDelayRight[i] = new CircularBuffer<Sample>(stereoSpread);
		combLP_[i] = new OnePoleFilter<Sample>(Sample(0.5));
	}
	for (size_t i = 0u; i < static_cast<size_t>(this->nAllpasses); i++)
	{
		allPassDelay_[i] = new CircularBuffer<Sample>(aDelayLengths[i]);
		allPassDelayRight[i] = new CircularBuffer<Sample>(stereoSpread);
	}

}

freeverbEffect::~freeverbEffect()
{
	for (unsigned int i = 0u; i < this->nCombs; i++)
	{
		delete combDelay_[i];
		delete combLP_[i];
		delete combDelayRight[i];
	}
	for (unsigned int i = 0u; i < this->nAllpasses; i++)
	{
		delete allPassDelay_[i];
		delete allPassDelayRight[i];
	}
}

void freeverbEffect::resetEffect()
{
	for (size_t i = 0u; static_cast<int>(i) < this->nCombs; i++)
	{
		combDelay_[i]->reset();
		combDelayRight[i]->reset();
	}
	for (size_t i = 0u; i < static_cast<size_t>(this->nAllpasses); i++)
	{
		allPassDelay_[i]->reset();
		allPassDelayRight[i]->reset();
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
	InterpolatedValue<float>& roomSize = getInterpolatedParameter(0);
	InterpolatedValue<float>& width = getInterpolatedParameter(2);

	const bool mode = getInterpolatedParameter(3).get() > 0.5 ? true : false;

	float damping = getInterpolatedParameter(1).get();
	damping = mode ? 0.f : damping * scaleDamp;
	const Sample currentRoomSize = Sample(mode ? 1.f : (roomSize.get() * scaleRoom) + offsetRoom);
	InterpolatedValue<float>& dry = getInterpolatedParameter(4);
	InterpolatedValue<float>& wet = getInterpolatedParameter(5);


	// set low pass filter for delay output
	for (unsigned int i = 0; i < nCombs; i++)
	{
		combLP_[i]->setParams(Sample(1.0 - damping), Sample(-1.0 * damping));
	}

	for (size_t i = 0; i < numberOfSamples; i++)
	{
		Sample fInput = buffer[i];
		Sample out(0.0);

		// Parallel LBCF filters
		for (unsigned int j = 0; j < nCombs; j++)
		{
			// Left channel
			Sample delayed = combDelay_[j]->get();
			Sample delayedRight = combDelayRight[j]->get();
			delayed.replaceRightChannel(delayedRight);
			Sample yn = fInput + (currentRoomSize * combLP_[j]->tick(delayed));
			Sample intermediate = combDelay_[j]->pushpop(yn);
			combDelayRight[j]->push(intermediate);
			out += yn;
		}

		// Series allpass filters
		for (unsigned int j = 0; j < nAllpasses; j++)
		{
			// Left channel
			Sample vn_m = allPassDelay_[j]->get();
			Sample vn_m_right = allPassDelayRight[j]->get();
			vn_m.replaceRightChannel(vn_m_right);
			Sample vn = out + g_ * vn_m;
			Sample intermediate = allPassDelay_[j]->pushpop(vn);
			allPassDelayRight[j]->push(intermediate);

			// calculate output
			out = vn_m - out;
		}

		// Mix output
		float wetIn = aux::decibelToLinear(wet.get());
		Sample wet1(wetIn * (width.get() * .5f + .5f));
		Sample wet2(wetIn * (.5f - width.get() * .5f));
		buffer[i] = wet1 * out + wet2 * out.flippedChannels() + buffer[i] * Sample(aux::decibelToLinear(dry.get()));
		nextSample();
	}
}
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


void freeverbEffect::process(Sample* buffer, size_t numberOfSamples)
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
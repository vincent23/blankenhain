#pragma once

#include "PluginBase.h"
#include "EffectBase.h"
#include "ParameterBundle.h"
#include "AuxFunc.h"
#include "CircularBuffer.h"
#include <algorithm>

class bh2_delay_effect : public EffectBase
{
public:
	bh2_delay_effect() : EffectBase(4, 512u), delayLine(size_t(aux::millisecToSamples(2502u)))
	{
		ParameterBundle* params = getPointerToParameterBundle();
		(params->getParameter(0)) = new ParameterWithProperties(0.f, NormalizedRange(-50.f, 50.f), "pan", "");
		(params->getParameter(1)) = new ParameterWithProperties(100.f, NormalizedRange(1.f, 2500.f), "length", "ms");
		(params->getParameter(2)) = new ParameterWithProperties(0.f, NormalizedRange(0.f, 1.5f), "feedback", "");
		(params->getParameter(3)) = new ParameterWithProperties(1.f, NormalizedRange(), "drywet", "");
	}

	void process(Sample* buffer, size_t numberOfSamples, size_t numberOfParameters, float* parameters)
	{
		float& pan = parameters[0];
		float& length = parameters[1];
		float& feedback = parameters[2];
		float& drywet = parameters[3];

		delayLine.setSize(static_cast<size_t>(aux::millisecToSamples(length)));
		for (size_t i = 0; i < numberOfSamples; i++)
		{
			double avg_ = avgValue(buffer[i]);
			Sample original = buffer[i];
			Sample line = delayLine.get();

			if (drywet > 0.5)
			{
				original *= Sample((1 - drywet) * 2.f);
			}
			else line *= Sample(drywet * 2.f);

			// Pan
			alignas(16) double lr[2];
			line.store_aligned(lr);
			lr[0] = lr[0] * (1.f - std::max(0.0f, pan));
			lr[1] = lr[1] * (1.f + std::min(0.0f, pan));
			line = load_aligned(lr);

			buffer[i] = original + line;

			delayLine.push(delayLine.get() * Sample(feedback) + Sample(avg_));
		}
	}
protected:
	CircularBuffer<Sample> delayLine;

};


class BH2_delay : public PluginBase
{
public:
	BH2_delay(audioMasterCallback audioMaster)
		: PluginBase(audioMaster, new bh2_delay_effect)
	{ }

	virtual void open()
	{
		setUniqueID(1991);
		isSynth(false);
	}

	virtual void close()
	{

	}
};
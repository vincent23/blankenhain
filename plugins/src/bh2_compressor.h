#pragma once

#define NUMBER_OF_PARAMETERS 5

#include "bh2_base.h"
#include "CircularBuffer.h"
#include <algorithm>

class bh2_compressor_effect : public BH2_effect_base
{
protected:
	CircularBuffer<Sample> delayLine;
	Sample envelope;
public:
	bh2_compressor_effect() : BH2_effect_base(512u), delayLine(aux::millisecToSamples(2502u))
	{
		this->currentParameters = new float[NUMBER_OF_PARAMETERS];
		this->params = new ParameterBundle(NUMBER_OF_PARAMETERS);

		(params->getParameter(0)) = new ParameterWithProperties(1.f, NormalizedRange(0.01f, 12.f, 1.f), "ratio", "");
		(params->getParameter(1)) = new ParameterWithProperties(10.f, NormalizedRange(0.2f, 15.f, 0.5f), "release", "ms");
		(params->getParameter(2)) = new ParameterWithProperties(0.f, NormalizedRange(-48.f, 0.f, 4.5f), "threshold", "dB");
		(params->getParameter(3)) = new ParameterWithProperties(10.f, NormalizedRange(0.2f, 35.f, 1.f), "attack", "ms");
		(params->getParameter(4)) = new ParameterWithProperties(0.f, NormalizedRange(0.f, 100.f, 1.f), "limiterOn", "");

		//ratio = new FloatParameter(1.f, "ratio", 1.f, NormalizedRange(0.f, 12.f, 1.f));
		//release = new FloatParameter(10.f / 132.f, "release", 1.f, NormalizedRange(0.f, 100.f, 1.f));
		//threshold = new FloatParameter(0.f, "threshold", 1.f, NormalizedRange(-48.f, 3.f, 1.f));
		//attack = new FloatParameter(10.f, "attack", 1.f, NormalizedRange(0.f, 100.f, 1.f));
		//limiterOn = new BoolParameter("limiterOn", false);
		//envelope = new Sample();
		envelope = Sample(0.);
	}

	~bh2_compressor_effect()
	{
		for (size_t i = 0u; i < this->params->getNumberOfParameters(); i++) {
			if (params->getParameter(i) != nullptr) delete params->getParameter(i);
			params->getParameter(i) = nullptr;
		}
		if (params != nullptr) delete params;
		params = nullptr;
		if (currentParameters != nullptr) delete[] currentParameters;
		currentParameters = nullptr;

		//
	}

	void process(Sample* buffer, size_t numberOfSamples, size_t numberOfParameters, float* parameters)
	{
		float& ratio = parameters[0];
		float& release = parameters[1];
		float& threshold = parameters[2];
		float& attack = parameters[3];
		bool limiterIsOn = parameters[4] > 0.5 ? true : false;
		size_t attackTimeInSamples = aux::millisecToSamples(attack);
		delayLine.setSize(attackTimeInSamples);
		release = aux::millisecToSamples(release);
		float attackGain = exp(-1 / static_cast<float>(attackTimeInSamples));
		float releaseGain = exp(-1 / (release));
		float slope = 1 - (1 / ratio);

		//delayLine.setSize(static_cast<size_t>(aux::millisecToSamples(length)));
		for (size_t i = 0; i < numberOfSamples; i++)
		{
			Sample input = abs(buffer[i]);
			if (envelope.areBothSmaller(input))
			{
				envelope = input + Sample(static_cast<double>(attackGain)) * (envelope - input);
			}
			else
			{
				envelope = input + Sample(static_cast<double>(releaseGain)) * (envelope - input);
			}
			double envelopeValue = maxValue(envelope);
			float envelopeDb = aux::linearToDecibel(static_cast<float>(abs(envelopeValue)));
			if (envelopeDb < threshold)
			{
				envelopeDb = 0.f;
			}
			else
			{
				if (limiterIsOn)
				{
					envelopeDb = (threshold - envelopeDb);
				}
				else envelopeDb = slope * (threshold - envelopeDb);
			}
			buffer[i] = delayLine.pushpop(buffer[i]);
			buffer[i] *= Sample(static_cast<double>(aux::decibelToLinear(envelopeDb)));
		}
	}


};


class BH2_compressor : public BH2_base
{
public:
	BH2_compressor(audioMasterCallback audioMaster) :
		BH2_base(audioMaster, NUMBER_OF_PARAMETERS)
	{
		bh_base = new bh2_compressor_effect();
		vstparameters = new VSTParameterBundle(NUMBER_OF_PARAMETERS, bh_base->getPointerToParameterBundle());
	}

	~BH2_compressor()
	{
		if (bh_base != nullptr) delete bh_base;
		bh_base = nullptr;
		if (vstparameters != nullptr) delete vstparameters;
		vstparameters = nullptr;
	}

	virtual void open()
	{
		setUniqueID(1993);
		isSynth(false);
	}

	virtual void close()
	{

	}
};
#include "CompressorEffect.h"

#include "ParameterBundle.h"
#include "AuxFunc.h"

#include <algorithm>

CompressorEffect::CompressorEffect() : EffectBase(NUMBER_OF_PARAMETERS, 512u), delayLine(size_t(aux::millisecToSamples(2502u)))
{
	ParameterBundle* params = getPointerToParameterBundle();
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

void CompressorEffect::process(Sample* buffer, size_t numberOfSamples, size_t numberOfParameters, float* parameters)
{
	float& ratio = parameters[0];
	float& release = parameters[1];
	float& threshold = parameters[2];
	float& attack = parameters[3];
	bool limiterIsOn = parameters[4] > 0.5 ? true : false;
	size_t attackTimeInSamples = size_t(aux::millisecToSamples(attack));
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
#include "CompressorEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"

#include <algorithm>

CompressorEffect::CompressorEffect() : EffectBase(NUMBER_OF_PARAMETERS), delayLine(size_t(aux::millisecToSamples(2502u)))
{
	ParameterBundle* params = getPointerToParameterBundle();
	(params->getParameter(0)) = new FloatParameter(1.f, NormalizedRange(0.01f, 12.f, 1.f), "ratio", "");
	(params->getParameter(1)) = new FloatParameter(10.f, NormalizedRange(0.2f, 15.f, 0.5f), "release", "ms");
	(params->getParameter(2)) = new FloatParameter(0.f, NormalizedRange(-48.f, 0.f, 4.5f), "threshold", "dB");
	(params->getParameter(3)) = new FloatParameter(10.f, NormalizedRange(0.2f, 35.f, 1.f), "attack", "ms");
	(params->getParameter(4)) = new FloatParameter(0.f, NormalizedRange(0.f, 100.f, 1.f), "limiterOn", "");

	//ratio = new FloatParameter(1.f, "ratio", 1.f, NormalizedRange(0.f, 12.f, 1.f));
	//release = new FloatParameter(10.f / 132.f, "release", 1.f, NormalizedRange(0.f, 100.f, 1.f));
	//threshold = new FloatParameter(0.f, "threshold", 1.f, NormalizedRange(-48.f, 3.f, 1.f));
	//attack = new FloatParameter(10.f, "attack", 1.f, NormalizedRange(0.f, 100.f, 1.f));
	//limiterOn = new BoolParameter("limiterOn", false);
	//envelope = new Sample();
	envelope = Sample(0.);
}

void CompressorEffect::process(Sample* buffer, size_t numberOfSamples)
{
	InterpolatedValue& ratio = getParameterValue(0);
	InterpolatedValue& release = getParameterValue(1);
	InterpolatedValue& threshold = getParameterValue(2);
	InterpolatedValue& attack = getParameterValue(3);
	bool limiterIsOn = getParameterValue(4).get() > 0.5 ? true : false;
	size_t attackTimeInSamples = size_t(aux::millisecToSamples(attack.get()));
	delayLine.setSize(attackTimeInSamples);

	//delayLine.setSize(static_cast<size_t>(aux::millisecToSamples(length)));
	for (size_t i = 0; i < numberOfSamples; i++)
	{
		float attackGain = exp(-1 / aux::millisecToSamples(attack.get()));
		float releaseGain = exp(-1 / (aux::millisecToSamples(release.get())));
		float slope = 1 - (1 / ratio.get());

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
		if (envelopeDb < threshold.get())
		{
			envelopeDb = 0.f;
		}
		else
		{
			if (limiterIsOn)
			{
				envelopeDb = (threshold.get() - envelopeDb);
			}
			else envelopeDb = slope * (threshold.get() - envelopeDb);
		}
		buffer[i] = delayLine.pushpop(buffer[i]);
		buffer[i] *= Sample(static_cast<double>(aux::decibelToLinear(envelopeDb)));
		nextSample();
	}
}
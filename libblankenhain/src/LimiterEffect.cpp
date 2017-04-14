#include "LimiterEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"
#include "Constants.h"

LimiterEffect::LimiterEffect()
	: EffectBase(5u)
	, lookaheadDelay(static_cast<unsigned int>(constants::sampleRate * 1e-3)) // constant lookahead of 1 ms
{
	ParameterBundle* params = getPointerToParameterBundle();
	(params->getParameter(0)) = new FloatParameter(1.f, NormalizedRange::fromMidpoint(0.01f, 3.f, 1000.f), "attack", "ms");
	(params->getParameter(1)) = new FloatParameter(20.f, NormalizedRange::fromMidpoint(1.f, 55.f, 3000.f), "release", "ms");
	(params->getParameter(2)) = new FloatParameter(0.f, NormalizedRange(-66.f, 6.f), "threshold", "dB");
	(params->getParameter(3)) = new FloatParameter(1.f, NormalizedRange::fromMidpoint(aux::samplesToMillisec(2u), 1.f, 20.f), "lookahead", "ms");
	(params->getParameter(4)) = new FloatParameter(0.f, NormalizedRange(-36.f, 36.f), "makeup", "dB");
}

//#pragma optimize( "gsty", off )

void LimiterEffect::process(Sample* buffer, size_t numberOfSamples, size_t currentTime)
{
	float attack = getInterpolatedParameter(0).get();
	float release = getInterpolatedParameter(1).get();
	InterpolatedValue<float>& threshold = getInterpolatedParameter(2);
	InterpolatedValue<float>& makeupGain = getInterpolatedParameter(4);
	float lookahead = getInterpolatedParameter(3).get();

	lookaheadDelay.setSize(static_cast<unsigned int>(aux::millisecToSamples(lookahead)));

	envelope.setTimes(attack, release);
	for (unsigned int i = 0; i < numberOfSamples; i++) 
	{
		envelope.nextPeakEnvelope(buffer[i]);
		float dbIn = aux::linearToDecibel(static_cast<float>(envelope.getCurrentEnvelope().maxValue()));
		float dbGain = 0.f;
		if (threshold.get() > dbIn)
			dbGain = makeupGain.get();
		else
			dbGain = makeupGain.get() - (dbIn - threshold.get());
		Sample delayed = lookaheadDelay.pushpop(buffer[i]);
		delayed *= Sample(aux::decibelToLinear(dbGain));
		buffer[i] = delayed;
		nextSample();
	}
}

//#pragma optimize( "gsty", on ) 

Sample LimiterEffect::getCurrentEnvelope() const
{
	return envelope.getCurrentEnvelope();
}
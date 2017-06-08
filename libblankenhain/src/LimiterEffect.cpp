#include "LimiterEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"
#include "Constants.h"

LimiterEffect::LimiterEffect()
	: EffectBase(5u)
	, lookaheadDelay(static_cast<unsigned int>(constants::sampleRate * 1e-3)) // constant lookahead of 1 ms
{
	ParameterBundle& params = getParameterBundle();
	params.initParameter(0, new FloatParameter(1.f, NormalizedRange::fromMidpoint(0.01f, 3.f, 1000.f), "attack", "ms"));
	params.initParameter(1, new FloatParameter(20.f, NormalizedRange::fromMidpoint(1.f, 55.f, 3000.f), "release", "ms"));
	params.initParameter(2, new FloatParameter(0.f, NormalizedRange(-66.f, 6.f), "threshold", "dB"));
	params.initParameter(3, new FloatParameter(1.f, NormalizedRange::fromMidpoint(aux::samplesToMillisec(2u), 1.f, 20.f), "lookahead", "ms"));
	params.initParameter(4, new FloatParameter(0.f, NormalizedRange(-36.f, 36.f), "makeup", "dB"));
}

//#pragma optimize( "gsty", off )

void LimiterEffect::process(Sample* buffer, size_t numberOfSamples, size_t currentTime)
{
	float attack = interpolatedParameters.get(0);
	float release = interpolatedParameters.get(1);
	float threshold = interpolatedParameters.get(2);
	float makeupGain = interpolatedParameters.get(4);
	float lookahead = interpolatedParameters.get(3);

	lookaheadDelay.setSize(static_cast<unsigned int>(aux::millisecToSamples(lookahead)));

	envelope.setTimes(attack, release);
	for (unsigned int i = 0; i < numberOfSamples; i++) 
	{
		envelope.nextPeakEnvelope(buffer[i]);
		float dbIn = aux::linearToDecibel(static_cast<float>(envelope.getCurrentEnvelope().maxValue()));
		float dbGain = 0.f;
		if (threshold > dbIn)
			dbGain = makeupGain;
		else
			dbGain = makeupGain - (dbIn - threshold);
		Sample delayed = lookaheadDelay.pushpop(buffer[i]);
		delayed *= Sample(aux::decibelToLinear(dbGain));
		buffer[i] = delayed;
	}
}

//#pragma optimize( "gsty", on ) 

Sample LimiterEffect::getCurrentEnvelope() const
{
	return envelope.getCurrentEnvelope();
}
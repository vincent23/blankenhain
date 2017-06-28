#include "CompressorEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"
#include "Constants.h"

CompressorEffect::CompressorEffect()
	: EffectBase(8u)
	//, lookaheadDelay(static_cast<unsigned int>(constants::sampleRate * 1e-3)) // constant lookahead of 1 ms
	, lookaheadDelay(64u)
{
	ParameterBundle& params = getParameterBundle();
	params.initParameter(0, new FloatParameter(1.f, NormalizedRange::fromMidpoint(0.01f, 3.f, 1000.f), "attack", "ms"));
	params.initParameter(1, new FloatParameter(20.f, NormalizedRange::fromMidpoint(1.f, 55.f, 3000.f), "release", "ms"));
	params.initParameter(2, new FloatParameter(0.f, NormalizedRange(-66.f, 6.f), "threshold", "dB"));
	params.initParameter(3, new FloatParameter(2.f, NormalizedRange::fromMidpoint(1.f, 2.f, 64.f), "ratio", ""));
	params.initParameter(4, new FloatParameter(0.f, NormalizedRange::fromMidpoint(0.f, 9.f, 18.f), "knee", "dB"));
	params.initParameter(5, new FloatParameter(1.f, NormalizedRange::fromMidpoint(aux::samplesToMillisec(2u), 1.f, 20.f), "lookahead", "ms"));
	params.initParameter(6, new FloatParameter(0.f, NormalizedRange(-36.f, 36.f), "makeup", "dB"));
	params.initParameter(7, new FloatParameter(1.f, NormalizedRange(0.f, 1.f), "envelope", "peak/rms"));
}

void CompressorEffect::process(Sample* buffer, size_t numberOfSamples, size_t currentTime)
{
	float attack = interpolatedParameters.get(0);
	float release = interpolatedParameters.get(1);
	bool rms = interpolatedParameters.get(7) >= .5f;
	float threshold = interpolatedParameters.get(2);
	float ratio = interpolatedParameters.get(3);
	float knee = interpolatedParameters.get(4);
	float makeupGain = interpolatedParameters.get(6);
	float lookahead = interpolatedParameters.get(5);

	// lookaheadDelay.setSize(static_cast<unsigned int>(aux::millisecToSamples(lookahead)));
	unsigned int lookaheadDelayLength = static_cast<unsigned int>(aux::millisecToSamples(lookahead));

	envelope.setTimes(attack, release);
	for (unsigned int i = 0; i < numberOfSamples; i++) {
		if (rms) {
			envelope.nextRmsEnvelope(buffer[i]);
		}
		else {
			envelope.nextPeakEnvelope(buffer[i]);
		}
		float dbIn = static_cast<float>(envelope.getCurrentEnvelope().maxValue());
		float dbGain = makeupGain + static_cast<float>(compressorGain(threshold, ratio, knee, dbIn));
		//Sample delayed = lookaheadDelay.pushpop(buffer[i]);
		Sample delayed = lookaheadDelay.get(lookaheadDelayLength);
		lookaheadDelay.push(buffer[i]);
		delayed *= Sample(aux::decibelToLinear(dbGain));;
		buffer[i] = delayed;
	}
}

Sample CompressorEffect::getCurrentEnvelope() const
{
	return envelope.getCurrentEnvelope();
}

double CompressorEffect::compressorGain(float threshold, float ratio, float knee, float dbIn)
{
	// probably this can be done in a smarter (branchless) way
	float kneeStart = static_cast<float>(threshold - knee);
	float kneeEnd = static_cast<float>(threshold + knee);
	if (dbIn < kneeStart) {
		return 0;
	}
	else if (dbIn < kneeEnd) {
		// quadratic bezier for knee
		//float t = (dbIn - kneeStart) / kneeWidth;
		//float a = kneeStart;
		//float b = threshold;
		//float c = threshold + knee / ratio;
		//float ab = a * (1.f - t) + b * t;
		//float bc = b * (1.f - t) + c * t;
		//dbOut = ab * (1.f - t) + bc * t - dbIn;
		// simplified version of the above:
		// TODO maybe this can be simplified even more due to the subtracted dbIn
		float t = (dbIn - kneeStart) / knee * .5f;
		return threshold + knee * (t * t * (1. / ratio - 1.f) + 2.f * t - 1.f) - dbIn;
	}
	else {
		return (threshold - dbIn) * (1. - 1. / ratio);
	}
}
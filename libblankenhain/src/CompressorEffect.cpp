#include "CompressorEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"
#include "Constants.h"

CompressorEffect::CompressorEffect()
	: EffectBase(8u)
	, lookaheadDelay(static_cast<unsigned int>(constants::sampleRate * 1e-3)) // constant lookahead of 1 ms
{
	ParameterBundle* params = getPointerToParameterBundle();
	(params->getParameter(0)) = new FloatParameter(1.f, NormalizedRange::fromMidpoint(0.01f, 3.f, 1000.f), "attack", "ms");
	(params->getParameter(1)) = new FloatParameter(20.f, NormalizedRange::fromMidpoint(1.f, 55.f, 3000.f), "release", "ms");
	(params->getParameter(2)) = new FloatParameter(0.f, NormalizedRange(-66.f, 6.f), "threshold", "dB");
	(params->getParameter(3)) = new FloatParameter(2.f, NormalizedRange::fromMidpoint(1.f, 2.f, 64.f), "ratio", "");
	(params->getParameter(4)) = new FloatParameter(0.f, NormalizedRange::fromMidpoint(0.f, 9.f, 18.f), "knee", "dB");
	(params->getParameter(5)) = new FloatParameter(1.f, NormalizedRange::fromMidpoint(0.f, 1.f, 10.f), "lookahead", "ms");
	(params->getParameter(6)) = new FloatParameter(0.f, NormalizedRange(-36.f, 36.f), "makeup", "dB");
	(params->getParameter(7)) = new FloatParameter(1.f, NormalizedRange(0.f, 1.f), "envelope", "peak/rms");
}

void CompressorEffect::process(Sample* buffer, size_t numberOfSamples)
{
	float attack = getInterpolatedParameter(0).get();
	float release = getInterpolatedParameter(1).get();
	bool rms = getInterpolatedParameter(7).get() >= .5f;
	InterpolatedValue<float>& threshold = getInterpolatedParameter(2);
	InterpolatedValue<float>& ratio = getInterpolatedParameter(3);
	InterpolatedValue<float>& knee = getInterpolatedParameter(4);
	InterpolatedValue<float>& makeupGain = getInterpolatedParameter(6);
	// TODO set lookahead
	envelope.setTimes(attack, release);
	for (unsigned int i = 0; i < numberOfSamples; i++) {
		if (rms) {
			envelope.getRmsEnvelope(buffer[i]);
		}
		else {
			envelope.getPeakEnvelope(buffer[i]);
		}
		float dbIn = envelope.getCurrentEnvelope().maxValue();
		float dbGain = makeupGain.get() + compressorGain(threshold.get(), ratio.get(), knee.get(), dbIn);
		Sample delayed = lookaheadDelay.pushpop(buffer[i]);
		delayed *= Sample(aux::decibelToLinear(dbGain));;
		buffer[i] = delayed;
		nextSample();
	}
}

Sample CompressorEffect::getCurrentEnvelope() const
{
	return envelope.getCurrentEnvelope();
}

double CompressorEffect::compressorGain(double threshold, double ratio, double knee, double dbIn)
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
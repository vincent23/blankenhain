#include "CompressorEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"

#include <algorithm>

CompressorEffect::CompressorEffect() : EffectBase(8u)
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
	float attack = getParameterValue(0).get();
	float release = getParameterValue(1).get();
	bool rms = getParameterValue(7).get() >= .5f;
	InterpolatedValue& makeupGain = getParameterValue(6);
	envelope.setTimes(attack, release);
	for (unsigned int i = 0; i < numberOfSamples; i++) {
		if (rms) {
			envelope.getRmsEnvelope(buffer[i]);
		}
		else {
			envelope.getPeakEnvelope(buffer[i]);
		}
		buffer[i] *= Sample(aux::decibelToLinear(makeupGain.get()));
		nextSample();
	}
}
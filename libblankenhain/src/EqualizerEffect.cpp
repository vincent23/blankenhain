#include "EqualizerEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"

EqualizerEffect::EqualizerEffect() : EffectBase(5)
{
	ParameterBundle* params = getPointerToParameterBundle();
	(params->getParameter(0)) = new FloatParameter(0.f, NormalizedRange(-36.f, 12.f, 1.f), "low", "dB");
	(params->getParameter(1)) = new FloatParameter(0.f, NormalizedRange(-36.f, 12.f, 1.f), "mid", "dB");
	(params->getParameter(2)) = new FloatParameter(0.f, NormalizedRange(-36.f, 12.f, 1.f), "high", "dB");
	(params->getParameter(3)) = new FloatParameter(530.f, NormalizedRange(40.f, 20000.f), "LowFreq", "1/s");
	(params->getParameter(4)) = new FloatParameter(5700.f, NormalizedRange(40.f, 20000.f), "HighFreq", "1/s");

	es = new effects::equalizer::Sample_EQSTATE();
	es->init(100., 44100., 5000.);
}

EqualizerEffect::~EqualizerEffect()
{
	delete es;
}

void EqualizerEffect::process(Sample* buffer, size_t numberOfSamples)
{
	float low = getParameterValue(0).get();
	float mid = getParameterValue(1).get();
	float high = getParameterValue(2).get();
	float lowfreq = getParameterValue(3).get();
	float highfreq = getParameterValue(4).get();


	const double mixfreq = 44100;
	es->lf = Sample(2 * sin(3.14159265359 * ((double)lowfreq / mixfreq)));
	es->hf = Sample(2 * sin(3.14159265359 * ((double)highfreq / mixfreq)));
	es->lg = Sample(aux::decibelToLinear(low));
	es->mg = Sample(aux::decibelToLinear(mid));
	es->hg = Sample(aux::decibelToLinear(high));

	for (size_t i = 0; i < numberOfSamples; i++)
	{
		buffer[i] = es->doThreeBand(buffer[i]);
	}
	nextSample();
}
#include "FilterEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"

const unsigned int NUMBER_OF_PARAMETERS = 3u;

FilterEffect::FilterEffect() : EffectBase(NUMBER_OF_PARAMETERS)
{
	ParameterBundle* params = getPointerToParameterBundle();
	// Insert your stuff here
	(params->getParameter(0)) = new FloatParameter(0.f, NormalizedRange(0.00f, 5.f, 1.f), "resonance", "");
	(params->getParameter(1)) = new FloatParameter(120.f, NormalizedRange(40.f, 22000.f, 0.3f), "frequency", "hz");
	(params->getParameter(2)) = new FloatParameter(0.f, NormalizedRange(0.f, 1.f, 1.f), "style", "");

	//resonance = new FloatParameter(0.5, "Resonance", 0.5f, NormalizedRange(0.f, 5.f));
	//frequency = new FloatParameter((500.f), "Frequency", 0.5f, NormalizedRange(40.f, 22000.f, 0.3f));
}

void FilterEffect::process(Sample* buffer, size_t numberOfSamples)
{
	float resonance = getParameterValue(0).get();
	float frequency = getParameterValue(1).get();
	float style = getParameterValue(2).get();
	if (style < 0.25)
		filter.processHigh(buffer, numberOfSamples, frequency, resonance);
	else if (style < 0.5)
		filter.processLow(buffer, numberOfSamples, frequency, resonance);
	else if (style < 0.75)
		filter.processBand(buffer, numberOfSamples, frequency, resonance);
	else
		filter.processNotch(buffer, numberOfSamples, frequency, resonance);
	nextSample(numberOfSamples);
}
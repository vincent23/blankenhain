#include "FilterEffect.h"

#include "ParameterBundle.h"

const unsigned int NUMBER_OF_PARAMETERS = 3u;

FilterEffect::FilterEffect() : EffectBase(NUMBER_OF_PARAMETERS, 512u)
{
	ParameterBundle* params = getPointerToParameterBundle();
	// Insert your stuff here
	(params->getParameter(0)) = new ParameterWithProperties(0.f, NormalizedRange(0.00f, 5.f, 1.f), "resonance", "");
	(params->getParameter(1)) = new ParameterWithProperties(120.f, NormalizedRange(40.f, 22000.f, 0.3f), "frequency", "hz");
	(params->getParameter(2)) = new ParameterWithProperties(0.f, NormalizedRange(0.f, 1.f, 1.f), "style", "");

	//resonance = new FloatParameter(0.5, "Resonance", 0.5f, NormalizedRange(0.f, 5.f));
	//frequency = new FloatParameter((500.f), "Frequency", 0.5f, NormalizedRange(40.f, 22000.f, 0.3f));
}

void FilterEffect::process(Sample* buffer, size_t numberOfSamples, size_t numberOfParameters, float* parameters)
{
	float& resonance = parameters[0];
	float& frequency = parameters[1];
	float& style = parameters[2];
	if (style < 0.25)
		filter.processHigh(buffer, numberOfSamples, frequency, resonance);
	else if (style < 0.5)
		filter.processLow(buffer, numberOfSamples, frequency, resonance);
	else if (style < 0.75)
		filter.processBand(buffer, numberOfSamples, frequency, resonance);
	else
		filter.processNotch(buffer, numberOfSamples, frequency, resonance);
}
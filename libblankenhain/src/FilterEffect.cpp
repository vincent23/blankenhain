#include "FilterEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"

const unsigned int NUMBER_OF_PARAMETERS = 4u;

FilterEffect::FilterEffect() : EffectBase(NUMBER_OF_PARAMETERS)
{
	ParameterBundle* params = getPointerToParameterBundle();
	// Insert your stuff here
	(params->getParameter(0)) = new FloatParameter(0.7f, NormalizedRange::fromMidpoint(0.1f, 0.7f, 10.f), "Q", "");
	(params->getParameter(1)) = new FloatParameter(120.f, NormalizedRange::fromMidpoint(20.f, 850.f, 22000.f), "frequency", "Hz");
	(params->getParameter(2)) = new FloatParameter(0.f, NormalizedRange(0.f, 1.f, 1.f), "style", "");
	(params->getParameter(3)) = new FloatParameter(1.01f, NormalizedRange(0.f, 2.9999f, 1.f), "rolloff", "");

	//resonance = new FloatParameter(0.5, "Resonance", 0.5f, NormalizedRange(0.f, 5.f));
	//frequency = new FloatParameter((500.f), "Frequency", 0.5f, NormalizedRange(40.f, 22000.f, 0.3f));
}

void FilterEffect::process(Sample* buffer, size_t numberOfSamples)
{
	float Q = getInterpolatedParameter(0).get();
	float frequency = getInterpolatedParameter(1).get();
	float style = getInterpolatedParameter(2).get();
	int numberOfFilters = static_cast<int>(getInterpolatedParameter(3).get() + 1);
	if (numberOfFilters == 3) {
		// 3 means 4
		numberOfFilters = 4;
	}
	// remap Q
	Q = std::pow(Q, 1./ numberOfFilters);

	for (unsigned int filterIndex = 0; static_cast<int>(filterIndex) < numberOfFilters; filterIndex++) {
		Filter<Sample>& filter = filters[filterIndex];
		if (style < 0.25) {
			filter.setHighPass(frequency, Q);
		}
		else if (style < 0.5) {
			filter.setLowPass(frequency, Q);
		}
		else if (style < 0.75) {
			filter.setBandPass(frequency, Q);
		}
		else {
			filter.setNotch(frequency, Q);
		}
		for (unsigned int i = 0; i < numberOfSamples; i++) {
			buffer[i] = filter.tick(buffer[i]);
		}
	}
	nextSample(numberOfSamples);
}
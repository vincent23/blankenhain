#include "FilterEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "BhMath.h"

const unsigned int NUMBER_OF_PARAMETERS = 4u;

FilterEffect::FilterEffect() : EffectBase(NUMBER_OF_PARAMETERS)
{
	ParameterBundle& params = getParameterBundle();
	// Insert your stuff here
	params.initParameter(0, new FloatParameter(0.7f, NormalizedRange::fromMidpoint(0.1f, 0.7f, 10.f), "Q", ""));
	params.initParameter(1, new FloatParameter(120.f, NormalizedRange::fromMidpoint(20.f, 850.f, 22000.f), "frequency", "Hz"));
	params.initParameter(2, new FloatParameter(0.f, NormalizedRange(0.f, 1.f, 1.f), "style", ""));
	params.initParameter(3, new FloatParameter(1.01f, NormalizedRange(0.f, 2.9999f, 1.f), "rolloff", ""));

	//resonance = new FloatParameter(0.5, "Resonance", 0.5f, NormalizedRange(0.f, 5.f));
	//frequency = new FloatParameter((500.f), "Frequency", 0.5f, NormalizedRange(40.f, 22000.f, 0.3f));
}

void FilterEffect::process(Sample* buffer, size_t numberOfSamples, size_t currentTime)
{
	float Q = interpolatedParameters.get(0);
	float frequency = interpolatedParameters.get(1);
	float style = interpolatedParameters.get(2);
	int numberOfFilters = static_cast<int>(interpolatedParameters.get(3) + 1);
	if (numberOfFilters == 3) {
		// 3 means 4
		numberOfFilters = 4;
	}
	// remap Q
	Q = BhMath::pow(Q, 1.f/ static_cast<float>(numberOfFilters));

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
}
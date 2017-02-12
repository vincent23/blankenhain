#include "EqualizerEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"

EqualizerEffect::EqualizerEffect() : EffectBase(1 + 5 * numberOfFilters)
{
	ParameterBundle* params = getPointerToParameterBundle();
	(params->getParameter(0)) = new FloatParameter(100.f, NormalizedRange(0.f, 200.f), "scale", "%");
	for (unsigned int filterIndex = 0; filterIndex < numberOfFilters; filterIndex++) {
		std::string indexStr = std::to_string(filterIndex + 1);
		(params->getParameter(filterIndex * 5 + 1)) = new FloatParameter(0.f, NormalizedRange(0.f, 1.f), "on" + indexStr, "");
		(params->getParameter(filterIndex * 5 + 2)) = new FloatParameter(850.f, NormalizedRange::fromMidpoint(20.f, 850.f, 22000.f), "freq" + indexStr, "Hz");
		(params->getParameter(filterIndex * 5 + 3)) = new FloatParameter(0.f, NormalizedRange(-15.f, 15.f), "gain" + indexStr, "dB");
		(params->getParameter(filterIndex * 5 + 4)) = new FloatParameter(0.71f, NormalizedRange::fromMidpoint(.1f, 1.34f, 18.f), "Q" + indexStr, "");
		// types:
		// 0: highpass
		// 1: low shelf
		// 2: bell
		// 3: notch
		// 4: high shelf
		// 5: lowpass
		(params->getParameter(filterIndex * 5 + 5)) = new FloatParameter(0.f, NormalizedRange(0.f, 5.99f), "type" + indexStr, "");
	}
}

void EqualizerEffect::process(Sample* buffer, size_t numberOfSamples)
{
	float scale = getInterpolatedParameter(0).get() / 100.f;
	for (unsigned int filterIndex = 0; filterIndex < numberOfFilters; filterIndex++) {
		if (getInterpolatedParameter(filterIndex * 5 + 1).get() < .5f) {
			// filter is off
			continue;
		}
		float frequency = getInterpolatedParameter(filterIndex * 5 + 2).get();
		float gain = getInterpolatedParameter(filterIndex * 5 + 3).get() * scale;
		float Q = getInterpolatedParameter(filterIndex * 5 + 4).get();
		int type = (int)getInterpolatedParameter(filterIndex * 5 + 5).get();
		Filter<Sample>& filter = filters[filterIndex];
		switch (type) {
		case 0:
			filter.setHighPass(frequency, Q);
			break;
		case 1:
			filter.setLowShelf(frequency, Q, gain);
			break;
		case 2:
			filter.setBell(frequency, Q, gain);
			break;
		case 3:
			filter.setNotch(frequency, Q);
			break;
		case 4:
			filter.setHighShelf(frequency, Q, gain);
			break;
		case 5:
			filter.setLowPass(frequency, Q);
			break;
		}
		for (unsigned int sampleIndex = 0; sampleIndex < numberOfSamples; sampleIndex++) {
			buffer[sampleIndex] = filter.tick(buffer[sampleIndex]);
		}
	}
	nextSample(numberOfSamples);
}
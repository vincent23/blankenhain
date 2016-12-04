#include "EqualizerEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"

#include <string>

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
	float scale = getParameterValue(0).get() / 100.;
	for (unsigned int filterIndex = 0; filterIndex < numberOfFilters; filterIndex++) {
		if (getParameterValue(filterIndex * 5 + 1).get() < .5f) {
			// filter is off
			continue;
		}
		float frequency = getParameterValue(filterIndex * 5 + 2).get();
		float gain = getParameterValue(filterIndex * 5 + 3).get() * scale;
		float Q = getParameterValue(filterIndex * 5 + 4).get();
		int type = (int)getParameterValue(filterIndex * 5 + 5).get();
		Filter& filter = filters[filterIndex];
		switch (type) {
		case 0:
		case 3:
		case 5:
			filter.recomputeCoefficients(frequency, Q);
			break;
		case 1:
			filter.recomputeCoefficientsLowShelf(frequency, Q, gain);
			break;
		case 2:
			filter.recomputeCoefficientsBell(frequency, Q, gain);
			break;
		case 4:
			filter.recomputeCoefficientsHighShelf(frequency, Q, gain);
			break;
		}
		for (unsigned int sampleIndex = 0; sampleIndex < numberOfSamples; sampleIndex++) {
			switch (type) {
			case 0:
				buffer[sampleIndex] = filter.tickHigh(buffer[sampleIndex]);
				break;
			case 1:
				buffer[sampleIndex] = filter.tickLowShelf(buffer[sampleIndex]);
				break;
			case 2:
				buffer[sampleIndex] = filter.tickBell(buffer[sampleIndex]);
				break;
			case 3:
				buffer[sampleIndex] = filter.tickNotch(buffer[sampleIndex]);
				break;
			case 4:
				buffer[sampleIndex] = filter.tickHighShelf(buffer[sampleIndex]);
				break;
			case 5:
				buffer[sampleIndex] = filter.tickLow(buffer[sampleIndex]);
				break;
			}
		}
	}
	nextSample(numberOfSamples);
}
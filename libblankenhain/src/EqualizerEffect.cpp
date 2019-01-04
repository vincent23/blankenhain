#include "EqualizerEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"

EqualizerEffect::EqualizerEffect() : EffectBase(1 + 5 * numberOfEqualizerFilters)
{
	ParameterBundle& params = getParameterBundle();
	params.initParameter(0, new FloatParameter(100.f, NormalizedRange(0.f, 200.f), "scale", "%"));
	for (unsigned int filterIndex = 0; filterIndex < numberOfEqualizerFilters; filterIndex++) {
		BhString onName, freqName, gainName, qName, typeName;
#ifndef _VC_NODEFAULTLIB
		std::string indexStr = std::to_string(filterIndex + 1);
		onName = "on" + indexStr;
		freqName = "freq" + indexStr;
		gainName = "gain" + indexStr;
		qName = "Q" + indexStr;
		typeName = "type" + indexStr;
#else
		onName = nullptr;
		freqName = nullptr;
		gainName = nullptr;
		qName = nullptr;
		typeName = nullptr;
#endif
		params.initParameter(filterIndex * 5 + 1, new BoolParameter(false, onName));
		params.initParameter(filterIndex * 5 + 2, new FloatParameter(850.f, NormalizedRange::fromMidpoint(20.f, 850.f, 22000.f), freqName, "Hz"));
		params.initParameter(filterIndex * 5 + 3, new FloatParameter(0.f, NormalizedRange(-15.f, 15.f), gainName, "dB"));
		params.initParameter(filterIndex * 5 + 4, new FloatParameter(0.71f, NormalizedRange::fromMidpoint(.1f, 1.34f, 18.f), qName, ""));
		BhString filterStyles[6] = { "High pass", "Low shelf", "Bell", "Notch", "High shelf", "Low pass" };
		params.initParameter(filterIndex * 5 + 5, new OptionParameter(6u, filterStyles, typeName, ""));
	}
}

void EqualizerEffect::process(Sample* buffer, size_t numberOfSamples, size_t currentTime)
{
	float scale = interpolatedParameters.get(0) / 100.f;
	for (unsigned int filterIndex = 0; filterIndex < numberOfEqualizerFilters; filterIndex++) {
		if (interpolatedParameters.get(filterIndex * 5 + 1) < .5f) {
			// filter is off
			continue;
		}
		float frequency = interpolatedParameters.get(filterIndex * 5 + 2);
		float gain = interpolatedParameters.get(filterIndex * 5 + 3) * scale;
		float Q = interpolatedParameters.get(filterIndex * 5 + 4);
		int type = (int)interpolatedParameters.get(filterIndex * 5 + 5);
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
}
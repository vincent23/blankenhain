#include "BitcrushEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"

#include <algorithm>

BitcrushEffect::BitcrushEffect() : EffectBase(3)
{
	ParameterBundle* params = getPointerToParameterBundle();
	(params->getParameter(0)) = new FloatParameter(0.5f, NormalizedRange(), "Bitcrush", "");
	(params->getParameter(1)) = new FloatParameter(0.f, NormalizedRange(), "Downsample", "");
	(params->getParameter(2)) = new FloatParameter(100.f, NormalizedRange(0.f, 100.f), "Dry/Wet", "%");
}

void BitcrushEffect::process(Sample* buffer, size_t numberOfSamples)
{

	InterpolatedValue<float>& bitcrush = getInterpolatedParameter(0);
	InterpolatedValue<float>& downsample = getInterpolatedParameter(1);
	InterpolatedValue<float>& drywet = getInterpolatedParameter(2);

	int groupedSamples = std::min(static_cast<int>(std::max(1.f, downsample.get() * 100.f)), static_cast<int>(numberOfSamples));
	float bitdepth = 12.f * (1.f - bitcrush.get()) + 1.f * bitcrush.get();
	int steps = static_cast<int>(exp2(bitdepth));

	for (size_t sample = 0; sample < numberOfSamples - groupedSamples; sample += groupedSamples)
	{
		Sample averagedSample(0., 0.);
		for (int i = 0; i < groupedSamples; i++)
		{
			averagedSample += buffer[i + sample] / Sample((double)groupedSamples);
		}

		Sample discretizedSample = discretize(averagedSample * Sample(static_cast<double>(steps)));
		discretizedSample /= Sample(static_cast<double>(steps));

		for (int i = 0; i < groupedSamples; i++) {
			Sample sampleValue = buffer[i + sample];
			buffer[i + sample] = sampleValue * Sample(1. - drywet.get(sample)) + discretizedSample * Sample(drywet.get(sample));
		}
	}

	Sample averagedSample(0., 0.);
	for (size_t i = static_cast<size_t>(numberOfSamples / groupedSamples) * groupedSamples; i < numberOfSamples; i++)
	{
		averagedSample += buffer[i] / Sample(static_cast<double>((numberOfSamples % groupedSamples)));
	}

	Sample discretizedSample = discretize(averagedSample * Sample(static_cast<double>(steps)));
	discretizedSample /= Sample(static_cast<double>(steps));

	for (size_t i = static_cast<size_t>(numberOfSamples / groupedSamples) * groupedSamples; i < numberOfSamples; i++)
	{
		Sample sampleValue = buffer[i];
		buffer[i] = sampleValue * Sample(1. - drywet.get(i)) + discretizedSample * Sample(drywet.get(i));
	}

	nextSample(numberOfSamples);
}

Sample BitcrushEffect::discretize(Sample const& sample)
{
	alignas(16) double lr[2];
	sample.store_aligned(lr);
	lr[0] = static_cast<double>(static_cast<int>(lr[0]));
	lr[1] = static_cast<double>(static_cast<int>(lr[1]));
	return load_aligned(lr);
}
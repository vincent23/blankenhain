#include "BitcrushEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"
#include "BhMath.h"

BitcrushEffect::BitcrushEffect() : EffectBase(4)
{
	ParameterBundle* params = getPointerToParameterBundle();

	params->getParameter(0) = new FloatParameter(100.f, NormalizedRange(1.f, 100.f, 0.15f), "Bitcrush", "");
	params->getParameter(1) = new FloatParameter(0.f, NormalizedRange(), "Downsample", "");
	params->getParameter(2) = new FloatParameter(0.f, NormalizedRange(), "Dry/Wet", "%");
	params->getParameter(3) = new BoolParameter(true, "averageDownsampling");
}

void BitcrushEffect::process(Sample* buffer, size_t numberOfSamples, size_t currentTime)
{
	const unsigned int bitcrush = static_cast<unsigned int>(getInterpolatedParameter(0).get());
	const float downsample = getInterpolatedParameter(1).get();
	const float drywet = getInterpolatedParameter(2).get();
	const bool avgDownsample = getInterpolatedParameter(3).get() == 1.f;

	unsigned int groupedSamples = aux::min(static_cast<unsigned int>(aux::max(1.f, downsample * static_cast<float>(numberOfSamples - 1u))), numberOfSamples);
	int steps = bitcrush;

	// Processing in groups of samples to perform downsampling. 
	// numberOfSamples can be arbitrary, so first we take care of all "full" groups 
	// according to our groupsize "groupedSamples".

	for (size_t sample = 0; sample < numberOfSamples - groupedSamples; sample += groupedSamples)
	{
		Sample averagedSample(0., 0.);
		if (avgDownsample)
		{
			for (size_t i = 0; i < groupedSamples; i++)
			{
				averagedSample += buffer[i + sample] / Sample((double)groupedSamples);
			}
		}
		else
			averagedSample = buffer[sample];

		Sample discretizedSample = discretize(averagedSample * Sample(static_cast<double>(steps)));
		discretizedSample /= Sample(static_cast<double>(steps));

		for (size_t i = 0; i < groupedSamples; i++) 
		{
			Sample sampleValue = buffer[i + sample];
			buffer[i + sample] = sampleValue * Sample(1. - drywet) + discretizedSample * Sample(drywet);
		}
	}

	// Take care of the remaining samples

	Sample averagedSample(0., 0.);
	if (avgDownsample)
	{
		for (size_t i = static_cast<size_t>(numberOfSamples / groupedSamples) * groupedSamples; i < numberOfSamples; i++)
		{
			averagedSample += buffer[i] / Sample(static_cast<double>((numberOfSamples % groupedSamples)));
		}
	}
	else
		averagedSample = buffer[static_cast<size_t>(numberOfSamples / groupedSamples) * groupedSamples];

	Sample discretizedSample = discretize(averagedSample * Sample(static_cast<double>(steps)));
	discretizedSample /= Sample(static_cast<double>(steps));

	for (size_t i = static_cast<size_t>(numberOfSamples / groupedSamples) * groupedSamples; i < numberOfSamples; i++)
	{
		Sample sampleValue = buffer[i];
		buffer[i] = sampleValue * Sample(1. - drywet) + discretizedSample * Sample(drywet);
	}

	nextSample(numberOfSamples);
}

Sample BitcrushEffect::discretize(Sample const& sample)
{
	alignas(16) floatType lr[2];
	sample.store_aligned(lr);
	lr[0] = static_cast<floatType>(static_cast<int>(lr[0]));
	lr[1] = static_cast<floatType>(static_cast<int>(lr[1]));
	return Sample(lr[0], lr[1]);
}
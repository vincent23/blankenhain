#include "BitcrushEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"
#include "BhMath.h"

BitcrushEffect::BitcrushEffect() : EffectBase(3)
{
	ParameterBundle& params = getParameterBundle();

	params.initParameter(0, new FloatParameter(16.f, NormalizedRange(1.f, 16.f), "Bitcrush", "bits"));
	params.initParameter(1, new FloatParameter(1.f, NormalizedRange(1.f, 30.f), "Downsample", "samples"));
	params.initParameter(2, new FloatParameter(1.f, NormalizedRange(), "Dry/Wet", "%"));
}

void BitcrushEffect::process(Sample* buffer, size_t numberOfSamples, size_t currentTime)
{
	const unsigned int bitcrush = static_cast<unsigned int>(interpolatedParameters.get(0));
	const int downsample = static_cast<int>(interpolatedParameters.get(1));
	const float drywet = interpolatedParameters.get(2);

	int steps1 = 1 << (bitcrush - 1);
	int steps2 = 1 << bitcrush;
	const Sample steps1Sample = Sample(static_cast<double>(steps1));
	const Sample steps1InverseSample = Sample(1.) / steps1Sample;
	const Sample steps2Sample = Sample(static_cast<double>(steps2));
	const Sample steps2InverseSample = Sample(1.) / steps2Sample;

	float stepsFract = (interpolatedParameters.get(0) - static_cast<float>(bitcrush));
	float downsampleFract = (interpolatedParameters.get(1) - static_cast<float>(downsample));;

	for (size_t sample = 0; sample < numberOfSamples; sample++)
	{
		Sample discretizedSample1 = discretize(buffer[sample] * steps1Sample) * steps1InverseSample;
		Sample discretizedSample2 = discretize(buffer[sample] * steps2Sample) * steps2InverseSample;
		Sample discretizedSample = discretizedSample1 * Sample(stepsFract) + discretizedSample2 * Sample(1.f - stepsFract);
		buffer[sample] = aux::mixDryWet(buffer[sample], discretizedSample, drywet);
		if (discardedSamples >= (downsample - 1))
		{
			lastSample1 = buffer[sample];
			discardedSamples = 0;
		}
		else {
			if (discardedSamples >= (downsample - 2))
			{
				lastSample2 = buffer[sample];
			}
			buffer[sample] = lastSample2 * Sample(downsampleFract) + lastSample1 * Sample(1.f - downsampleFract);
			discardedSamples++;
		}
	}
}

Sample BitcrushEffect::discretize(Sample const& sample)
{
	alignas(16) floatType lr[2];
	sample.store_aligned(lr);
	lr[0] = static_cast<floatType>(static_cast<int>(lr[0]));
	lr[1] = static_cast<floatType>(static_cast<int>(lr[1]));
	return Sample(lr[0], lr[1]);
}
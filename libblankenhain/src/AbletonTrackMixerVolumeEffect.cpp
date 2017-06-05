#include "AbletonTrackMixerVolumeEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"

AbletonTrackMixerVolumeEffect::AbletonTrackMixerVolumeEffect() : EffectBase(1u, false)
{
	ParameterBundle* params = getPointerToParameterBundle();
	// Note, volume in xml file is between 0 and 2 appareantly
	(params->getParameter(0)) = new FloatParameter(0.f, NormalizedRange(0.f,2.f), "volumeFromXMLFile", "linear");
}

void AbletonTrackMixerVolumeEffect::process(Sample* buffer, size_t numberOfSamples, size_t currentTime)
{
	InterpolatedValue<float> const& currentVolumeAsMultiplier = getInterpolatedParameter(0);

	const float valueBefore = currentVolumeAsMultiplier.get();
	const float valueAfter = currentVolumeAsMultiplier.get(numberOfSamples);
	InterpolatedValue<float> volume(valueBefore, valueBefore, numberOfSamples); // TODO
	//InterpolatedValue<float> volume(valueBefore, valueAfter, numberOfSamples); // TODO
	for (size_t bufferIteration = 0; bufferIteration < numberOfSamples; bufferIteration++)
	{
		//float transformedMultiplier = 0.00956 * BhMath::exp(22048.f / 2.f * volume.get() / 4973.87572f) - 0.01101f;
		float transformedMultiplier = 0.00624 * BhMath::exp(((22048.f / 2.f * volume.get()) + 2124.69956) / 4973.87572f) - 0.01101f;
		transformedMultiplier *= (1.f / 0.78727f) * aux::decibelToLinear(6.f);

		if (transformedMultiplier > 0.f)
			buffer[bufferIteration] *= Sample(transformedMultiplier);
		else
			buffer[bufferIteration] = Sample(0.f);

		volume.next();
	}
}

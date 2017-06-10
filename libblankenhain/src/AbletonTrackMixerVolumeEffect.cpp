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
	InterpolatedValue<float> volume(valueBefore, valueAfter, numberOfSamples);
	for (size_t bufferIteration = 0; bufferIteration < numberOfSamples; bufferIteration++)
	{
		Sample multiplier;

		float fvol = volume.get();
		if (fvol > 1.f)
		{
			float mult = -6.02413 + 2.71507e-4 * ((fvol - 1.f) * 22050.f);
			multiplier = Sample(aux::decibelToLinear(mult + 6.f));
		}
		else if (fvol >= 0.5f)
		{
			float mult2 = -16.05759 + 24.23926 * fvol - 8.20311 * fvol * (fvol);
			multiplier = Sample(aux::decibelToLinear(mult2));
		}
		else
		{
			float x = (fvol * 22050.f);
			float mult = -67.17493 + 0.00855 * x - 4.70518e-7 * x * x + 1.01315e-11 * x * x * x;
			mult += 12.f;
			if (mult > -6.f)
				mult = -6.f;
			multiplier = Sample(aux::decibelToLinear(mult));
		}
		buffer[bufferIteration] *= multiplier;
		volume.next();
	}
}

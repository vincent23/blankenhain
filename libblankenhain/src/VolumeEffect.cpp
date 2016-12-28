#include "VolumeEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"

VolumeEffect::VolumeEffect() : EffectBase(3u)
{
	ParameterBundle* params = getPointerToParameterBundle();
	(params->getParameter(0)) = new FloatParameter(0.f, NormalizedRange(-120.f, 12.f, 5.f), "volumeL", "dB");
	(params->getParameter(1)) = new FloatParameter(0.f, NormalizedRange(-120.f, 12.f, 5.f), "volumeR", "dB");
	(params->getParameter(2)) = new FloatParameter(1.f, NormalizedRange(), "coupling", "bool");
}

void VolumeEffect::process(Sample* buffer, size_t numberOfSamples)
{
	InterpolatedValue<float>& currentVolumeL = getInterpolatedParameter(0);
	InterpolatedValue<float>& currentVolumeR = getInterpolatedParameter(1);
	bool coupling = getInterpolatedParameter(2).get() > 0.5 ? true : false;


	if (coupling)
	{
		for (size_t bufferIteration = 0; bufferIteration < numberOfSamples; bufferIteration++)
		{
			buffer[bufferIteration] = buffer[bufferIteration] * aux::decibelToLinear(currentVolumeL.get());
			nextSample();
		}
	}
	else
	{
		alignas(16) double currentBuffer[2];
		for (
			size_t bufferIteration = 0u;
			bufferIteration < numberOfSamples;
			bufferIteration++
			)
		{
			buffer[bufferIteration].store_aligned(currentBuffer);
			currentBuffer[0] = currentBuffer[0] * aux::decibelToLinear(currentVolumeL.get());
			currentBuffer[1] = currentBuffer[1] * aux::decibelToLinear(currentVolumeR.get());
			buffer[bufferIteration].load_aligned(currentBuffer);
			nextSample();
		}
	}
}
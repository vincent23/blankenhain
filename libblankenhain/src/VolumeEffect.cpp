#include "VolumeEffect.h"

#include "ParameterBundle.h"
#include "AuxFunc.h"

VolumeEffect::VolumeEffect() : EffectBase(3u, 512u)
{
	ParameterBundle* params = getPointerToParameterBundle();
	(params->getParameter(0)) = new ParameterWithProperties(0.f, NormalizedRange(-120.f, 12.f, 5.f), "volumeL", "dB");
	(params->getParameter(1)) = new ParameterWithProperties(0.f, NormalizedRange(-120.f, 12.f, 5.f), "volumeR", "dB");
	(params->getParameter(2)) = new ParameterWithProperties(1.f, NormalizedRange(), "coupling", "bool");
}

void VolumeEffect::process(Sample* buffer, size_t sampleFrames, size_t numberOfParameters, float* parameters)
{
	float& currentVolumeL = parameters[0];
	float& currentVolumeR = parameters[1];
	bool coupling = parameters[2] > 0.5 ? true : false;

	alignas(16) double currentBuffer[2];
	if (coupling)
	{
		for (size_t bufferIteration = 0; bufferIteration < sampleFrames; bufferIteration++)
		{
			buffer[bufferIteration] = buffer[bufferIteration] * aux::decibelToLinear(currentVolumeL);
		}
	}
	else
	{
		for (
			size_t bufferIteration = 0u;
			bufferIteration < sampleFrames;
			bufferIteration++
			)
		{
			buffer[bufferIteration].store_aligned(currentBuffer);
			currentBuffer[0] = currentBuffer[0] * aux::decibelToLinear(currentVolumeL);
			currentBuffer[1] = currentBuffer[1] * aux::decibelToLinear(currentVolumeR);
			buffer[bufferIteration].load_aligned(currentBuffer);
		}
	}
}
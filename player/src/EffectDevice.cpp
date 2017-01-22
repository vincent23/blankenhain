#include "EffectDevice.h"

#include "EffectBase.h"
#include "ParameterBundle.h"
#include "ParameterTrack.h"
#include "SongInfo.h"

EffectDevice::EffectDevice(unsigned int blockSize, EffectBase& effect_, ParameterTrack* parameterValues_)
	: Device(blockSize)
	, effect(effect_)
	, parameterValues(parameterValues_)
{}

Sample* EffectDevice::process(SongInfo& songInfo, const Sample* input, unsigned int numberOfSamples, unsigned int globalSamplePosition)
{
	if (effect.effectUsesTempoData())
	{
		effect.setTempoData(songInfo.bpm, globalSamplePosition);
	}

	ParameterBundle* parameters = effect.getPointerToParameterBundle();
	for (unsigned int parameterIndex = 0; parameterIndex < effect.getNumberOfParameters(); parameterIndex++)
	{
		float targetValue = parameterValues[parameterIndex].getValueAt(globalSamplePosition);
		parameters->getParameter(parameterIndex)->setTargetValueNormalized(targetValue);
	}
	for (unsigned int sampleIndex = 0; sampleIndex < numberOfSamples; sampleIndex++)
	{
		outputBuffer[sampleIndex] = input[sampleIndex];
	}
	effect.processBlock(outputBuffer, numberOfSamples);
	return outputBuffer;
}
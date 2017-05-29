#include "EffectDevice.h"

#include "EffectBase.h"
#include "ParameterBundle.h"
#include "ParameterTrack.h"
#include "SongInfo.h"
#include "Constants.h"

EffectDevice::EffectDevice(EffectBase& effect_, ParameterTrack* parameterValues_)
	: effect(effect_)
	, parameterValues(parameterValues_)
{
	ParameterBundle* parameters = effect.getPointerToParameterBundle();
	for (unsigned int parameterIndex = 0; parameterIndex < effect.getNumberOfParameters(); parameterIndex++)
	{
		float targetValue = parameterValues[parameterIndex].getCurrentValueAndAdvance(0);
		parameters->getParameter(parameterIndex)->setCurrentValueNormalized(targetValue);
	}
}

Sample* EffectDevice::process(SongInfo& songInfo, const Sample* input, unsigned int globalSamplePosition)
{
	for (unsigned int sampleIndex = 0; sampleIndex < constants::blockSize; sampleIndex++)
	{
		outputBuffer[sampleIndex] = input[sampleIndex];
	}
	const unsigned int onIndex = effect.getNumberOfParameters();
	const bool isOn = parameterValues[onIndex].getCurrentDiscreteValueAndAdvance(globalSamplePosition) > .5f;
	if (isOn)
	{
		if (effect.effectUsesTempoData())
		{
			effect.setTempoData(songInfo.bpm, globalSamplePosition);
		}

		ParameterBundle* parameters = effect.getPointerToParameterBundle();
		for (unsigned int parameterIndex = 0; parameterIndex < effect.getNumberOfParameters(); parameterIndex++)
		{
			if (parameterValues[parameterIndex].numberOfPoints > 1) {
				float targetValue = parameterValues[parameterIndex].getCurrentValueAndAdvance(globalSamplePosition);
				parameters->getParameter(parameterIndex)->setTargetValueNormalized(targetValue);
			}
		}
		effect.processBlock(outputBuffer, constants::blockSize);
	}
	return outputBuffer;
}
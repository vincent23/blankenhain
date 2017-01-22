#include "ChainDevice.h"

Sample* ChainDevice::process(SongInfo& songInfo, const Sample* input, unsigned int numberOfSamples, unsigned int globalSamplePosition)
{
	const Sample* previousOutput = input;
	for (unsigned int deviceIndex = 0; deviceIndex < numberOfDevices; deviceIndex++)
	{
		Device& device = devices[deviceIndex];
		previousOutput = device.process(songInfo, previousOutput, numberOfSamples, globalSamplePosition);
	}
	for (unsigned int samplePosition = 0; samplePosition < numberOfSamples; samplePosition++)
	{
		outputBuffer[samplePosition] = previousOutput[samplePosition];
	}
	return outputBuffer;
}
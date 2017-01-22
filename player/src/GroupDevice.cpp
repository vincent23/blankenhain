#include "GroupDevice.h"

Sample* GroupDevice::process(SongInfo& songInfo, const Sample* input, unsigned int numberOfSamples, unsigned int globalSamplePosition)
{
	for (unsigned int samplePosition = 0; samplePosition < numberOfSamples; samplePosition++)
	{
		outputBuffer[samplePosition] = Sample(0);
	}
	for (unsigned int deviceIndex = 0; deviceIndex < numberOfDevices; deviceIndex++)
	{
		Device& device = devices[deviceIndex];
		Sample* output = device.process(songInfo, input, numberOfSamples, globalSamplePosition);
		for (unsigned int samplePosition = 0; samplePosition < numberOfSamples; samplePosition++)
		{
			outputBuffer[samplePosition] += output[samplePosition];
		}
	}
	return outputBuffer;
}
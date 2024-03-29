#include "GroupDevice.h"

#include "Constants.h"

Sample* GroupDevice::process(SongInfo& songInfo, const Sample* input, unsigned int globalSamplePosition)
{
	for (unsigned int samplePosition = 0; samplePosition < constants::blockSize; samplePosition++)
	{
		outputBuffer[samplePosition] = Sample(0);
	}
	for (unsigned int deviceIndex = 0; deviceIndex < numberOfDevices; deviceIndex++)
	{
		Device& device = *(devices[deviceIndex]);
		Sample* output = device.process(songInfo, input, globalSamplePosition);
		for (unsigned int samplePosition = 0; samplePosition < constants::blockSize; samplePosition++)
		{
			outputBuffer[samplePosition] += output[samplePosition];
		}
	}
	return outputBuffer;
}
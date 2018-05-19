#include "ChainDevice.h"

#include "Constants.h"

Sample* ChainDevice::process(SongInfo& songInfo, const Sample* input, unsigned int globalSamplePosition)
{
	const Sample* previousOutput = input;
	for (unsigned int deviceIndex = 0; deviceIndex < numberOfDevices; deviceIndex++)
	{
		Device& device = *(devices[deviceIndex]);
		previousOutput = device.process(songInfo, previousOutput, globalSamplePosition);
	}
	for (unsigned int samplePosition = 0; samplePosition < constants::blockSize; samplePosition++)
	{
		outputBuffer[samplePosition] = previousOutput[samplePosition];
	}
	return outputBuffer;
}
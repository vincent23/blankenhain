#include "ChainDevice.h"

Sample* ChainDevice::process(MidiTrack* tracks, const Sample* input, unsigned int numberOfSamples)
{
	const Sample* previousOutput = input;
	for (unsigned int deviceIndex = 0; deviceIndex < numberOfDevices; deviceIndex++)
	{
		Device& device = devices[deviceIndex];
		previousOutput = device.process(tracks, previousOutput, numberOfSamples);
	}
	for (unsigned int samplePosition = 0; samplePosition < numberOfSamples; samplePosition++)
	{
		outputBuffer[samplePosition] = previousOutput[samplePosition];
	}
	return outputBuffer;
}
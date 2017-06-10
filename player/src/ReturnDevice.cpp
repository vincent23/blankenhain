#include "ReturnDevice.h"
#include "SongInfo.h"
#include "Constants.h"

ReturnDevice::ReturnDevice(unsigned int returnTrackIndex_)
	: returnTrackIndex(returnTrackIndex_)
{ }

Sample* ReturnDevice::process(SongInfo& songInfo, const Sample* input, unsigned int globalSamplePosition)
{
	Sample* returnBuffer = songInfo.sendBuffers + constants::parameterInterpolationLength * returnTrackIndex;
	for (unsigned int sampleIndex = 0; sampleIndex < constants::parameterInterpolationLength; sampleIndex++)
	{
		outputBuffer[sampleIndex] = returnBuffer[sampleIndex];
	}
	return outputBuffer;
}
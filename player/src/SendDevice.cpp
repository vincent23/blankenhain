#include "SendDevice.h"
#include "SongInfo.h"
#include "ParameterTrack.h"
#include "Constants.h"
#include "AuxFunc.h"
#include "InterpolatedValue.h"

SendDevice::SendDevice(ParameterTrack* parameterValues_)
	: parameterValues(parameterValues_)
{ }

Sample* SendDevice::process(SongInfo& songInfo, const Sample* input, unsigned int globalSamplePosition)
{
	for (unsigned int returnTrackIndex = 0; returnTrackIndex < songInfo.numberOfReturnTracks; returnTrackIndex++)
	{
		Sample* sendBuffer = songInfo.sendBuffers + constants::blockSize * returnTrackIndex;
		float send = parameterValues[returnTrackIndex].getCurrentValueAndAdvance(globalSamplePosition);
		float sendDb = sendToDb(send);
		float sendValueBefore = aux::decibelToLinear(sendDb);
		float sendValueAfter = aux::decibelToLinear(sendToDb(parameterValues[returnTrackIndex].getCurrentValueAndAdvance(globalSamplePosition + constants::blockSize)));
		InterpolatedValue<float> sendLinear(sendValueBefore, sendValueAfter, constants::blockSize);
		for (unsigned int samplePosition = 0; samplePosition < constants::blockSize; samplePosition++)
		{
			sendBuffer[samplePosition] += Sample(sendLinear.get()) * input[samplePosition];
			sendLinear.next();
		}
	}
	for (unsigned int sampleIndex = 0; sampleIndex < constants::blockSize; sampleIndex++)
	{
		outputBuffer[sampleIndex] = input[sampleIndex];
	}
	return outputBuffer;
}

float SendDevice::sendToDb(float send) const
{
	if (send >= .4) {
		// linear from [0.4, -24] to [1.0, 0.0]
		return 40.f * send - 40.f;
	}
	else {
		// 2rd degree polynomial fit
		return -215.73172328f * (send * send) + 211.12290386f * send - 73.9320852f;
	}
}
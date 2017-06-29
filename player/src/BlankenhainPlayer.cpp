#include "BlankenhainPlayer.h"

#include "Constants.h"
#include "Song.h"
#include "SongInfo.h"
#include "ChainDevice.h"
#include "Options.h"
#include "FpuState.h"

void BlankenhainPlayer::play(Song& song, float* audioBuffer)
{
	FpuState fpuState;

	// round blocksize up
	const unsigned int numberOfSamples = ((song.songInfo.lengthInSamples - 1) / constants::parameterInterpolationLength + 1) * constants::parameterInterpolationLength;
	Sample buffer[constants::parameterInterpolationLength];
	for (unsigned int i = 0; i < numberOfSamples; i += constants::parameterInterpolationLength)
	{
		Sample zero(0);
		for (unsigned int j = 0; j < constants::parameterInterpolationLength; j++)
		{
			buffer[j] = zero;
		}
		for (unsigned int j = 0; j < song.songInfo.numberOfReturnTracks * constants::parameterInterpolationLength; j++)
		{
			song.songInfo.sendBuffers[j] = zero;
		}
		Sample* output = song.master.process(song.songInfo, buffer, i);
		for (unsigned int j = 0; j < constants::parameterInterpolationLength; j++)
		{
			alignas(16) floatType sample[2];
			output[j].store_aligned(sample);
			audioBuffer[2 * (i + j)] = float(sample[0]);
			audioBuffer[2 * (i + j) + 1] = float(sample[1]);
		}
	}

}
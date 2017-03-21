#include "BlankenhainPlayer.h"

#include "Constants.h"
#include "Song.h"
#include "SongInfo.h"
#include "ChainDevice.h"

#define VC_EXTRALEAN
//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>

void BlankenhainPlayer::play(Song& song)
{
	// round blocksize up
	const unsigned int numberOfSamples = ((song.songInfo.lengthInSamples - 1) / constants::blockSize + 1) * constants::blockSize;
	float* audioBuffer = new float[numberOfSamples * 2];
	Sample buffer[constants::blockSize];
	for (unsigned int i = 0; i < numberOfSamples; i += constants::blockSize)
	{
		for (unsigned int j = 0; j < constants::blockSize; j++)
		{
			buffer[j] = Sample(0);
		}
		Sample* output = song.master.process(song.songInfo, buffer, i);
		for (unsigned int j = 0; j < constants::blockSize; j++)
		{
			alignas(16) double sample[2];
			output[j].store_aligned(sample);
			audioBuffer[2 * (i + j)] = sample[0];
			audioBuffer[2 * (i + j) + 1] = sample[1];
		}
	}

	const unsigned int sampleRate = 44100;
	const unsigned int blockAlignment = sizeof(float) * 2;
	const unsigned int totalBytes = numberOfSamples * blockAlignment;
	const unsigned int bytesPerSecond = sampleRate * blockAlignment;
	const unsigned int bitsPerSample = sizeof(float) * 8;

	HWAVEOUT audio_wave_out;
	WAVEHDR audio_wave_header;
	audio_wave_header = {
		(LPSTR)audioBuffer,
		totalBytes,
		0,
		0,
		0,
		0,
		0,
		0
	};
	WAVEFORMATEX wave_format = {
		WAVE_FORMAT_IEEE_FLOAT,
		/* channels        */ 2,
		/* samples/second  */ sampleRate,
		/* bytes/second    */ bytesPerSecond,
		/* block alignment */ blockAlignment,
		/* bits/sample     */ bitsPerSample,
		/* no extensions   */ 0
	};

	waveOutOpen(&audio_wave_out, WAVE_MAPPER, &wave_format, NULL, 0, CALLBACK_NULL);
	waveOutPrepareHeader(audio_wave_out, &audio_wave_header, sizeof(audio_wave_header));

	// play

	waveOutWrite(audio_wave_out, &audio_wave_header, sizeof(audio_wave_header));
	while (true);

	delete audioBuffer;
}
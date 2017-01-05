#include "gmSound.h"

#include "Sample.h"
#include <cinttypes>
#include <windows.h>

gmSound::gmSound(gmSoundRegion& region, HANDLE h, unsigned int targetNote)
	: interpolatedBufferSize(0u), interpolatedBuffer(nullptr), isLoopable(region.isLoopable), rootBuffer(nullptr),
	loopStart(0u), loopStop(0u), mRegion(region)
{
	rootBuffer = new Sample[region.sampleLength / 2u];

	int16_t* raw_sample = new int16_t[region.sampleLength / 2];
	if (SetFilePointer(h, region.startByte, NULL/*32 bit*/, FILE_BEGIN) != INVALID_SET_FILE_POINTER)
	{
		DWORD dwBytesRead;
		bool status = ReadFile(h, raw_sample, region.sampleLength, &dwBytesRead, NULL);

		int x = 0;
	}
	else {
		DWORD error = GetLastError();
		int x = 0;
	}

	for (unsigned int i = 0u; i < region.sampleLength / 2u; i++)
	{
		rootBuffer[i] = Sample((static_cast<float>(raw_sample[i]) - 0.5f) / 32767.5f);
	}

	delete raw_sample;



	// Now interpolation
	// playback speed via
	// http://math.stackexchange.com/questions/1205881/what-is-the-equation-for-figuring-out-the-change-in-pitch-from-changes-in-tempo

	float multiplciationLengthFactor = 1. / std::powf(2.f, (static_cast<float>(region.rootNote) - static_cast<float>(targetNote)) / -12.f);
	interpolatedBufferSize = static_cast<unsigned int>(region.sampleLength / (2 * multiplciationLengthFactor));
	interpolatedBufferSize = region.sampleLength / 2u;

	interpolatedBuffer = new Sample[interpolatedBufferSize];
	if (isLoopable)
	{
		//loopStart = static_cast<unsigned int>(region.loopStart / 2.f * multiplciationLengthFactor);
		//loopStop = static_cast<unsigned int>((region.loopStart + region.loopLength) / 2.f * multiplciationLengthFactor);
		loopStart = region.loopStart;
		loopStop = region.loopStart + region.loopLength;
	}

	for (unsigned int i = 0u; i < interpolatedBufferSize; i++)
	{
		//interpolatedBuffer[i] = rootBuffer[static_cast<unsigned int>(static_cast<float>(i) * multiplciationLengthFactor)];
		interpolatedBuffer[i] = rootBuffer[i];
	}

	delete[] rootBuffer;
	rootBuffer = nullptr;
}

gmSound::~gmSound()
{
	if (interpolatedBuffer != nullptr)
	{
		delete[] interpolatedBuffer;
		interpolatedBuffer = nullptr;
	}
	if (rootBuffer != nullptr)
	{
		delete[] rootBuffer;
		rootBuffer = nullptr;
	}
}
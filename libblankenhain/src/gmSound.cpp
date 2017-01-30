#include "gmSound.h"

#include "Sample.h"
#include "Constants.h"
#include <cinttypes>
#include <windows.h>

gmSound::gmSound(gmSoundRegion& region, HANDLE h, unsigned int targetNote)
	: interpolatedBufferSize(0u)
	, interpolatedBuffer(nullptr)
	, isLoopable(region.isLoopable)
	, loopStart(0u)
	, loopLength(0u)
{
	unsigned int inputSampleLength = region.sampleLength / 2;
	int16_t* rawSample = new int16_t[inputSampleLength];
	if (SetFilePointer(h, region.startByte, NULL/*32 bit*/, FILE_BEGIN) != INVALID_SET_FILE_POINTER)
	{
		DWORD dwBytesRead;
		bool status = ReadFile(h, rawSample, region.sampleLength, &dwBytesRead, NULL);
	}
	else {
		// TODO remove this in release
		DWORD error = GetLastError();
	}

	Sample* rootBuffer = new Sample[inputSampleLength];
	for (unsigned int i = 0u; i < inputSampleLength; i++)
	{
		rootBuffer[i] = Sample((static_cast<float>(rawSample[i]) - 0.5f) / 32767.5f);
	}
	delete[] rawSample;

	float pitchFactor = std::exp2(-(static_cast<float>(region.rootNote) - static_cast<float>(targetNote)) / 12.f);
	interpolatedBufferSize = inputSampleLength * 2 / pitchFactor;
	interpolatedBuffer = new Sample[interpolatedBufferSize];

	// simple linear interpolation
	// TODO replace with something better e.g. bandlimited resampling
	// maybe we can even try to do something to make the loops more seamless
	for (unsigned int i = 0; i < interpolatedBufferSize; i++) {
		float interpolatedIndex = static_cast<float>(i) * .5f * pitchFactor;
		unsigned int indexLower = static_cast<unsigned int>(interpolatedIndex);
		unsigned int indexUpper = indexLower + 1;
		if (indexUpper > inputSampleLength - 1) {
			indexUpper = inputSampleLength - 1;
		}
		float t = interpolatedIndex - static_cast<float>(indexLower);
		interpolatedBuffer[i] = rootBuffer[indexLower] * Sample(1.f - t) + rootBuffer[indexUpper] * Sample(t);
	}

	delete[] rootBuffer;

	loopStart = region.loopStart / pitchFactor * 2;
	loopLength = region.loopLength / pitchFactor * 2;
}

gmSound::~gmSound()
{
	if (interpolatedBuffer != nullptr)
	{
		delete[] interpolatedBuffer;
		interpolatedBuffer = nullptr;
	}
}
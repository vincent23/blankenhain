#include "gmSound.h"

#include "Sample.h"
#include "Constants.h"
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "BhMath.h"

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
		bool status = ReadFile(h, rawSample, region.sampleLength, &dwBytesRead, NULL) == 1;
	}
	else {
		// TODO remove this in release
		DWORD error = GetLastError();
	}

	float* rootBuffer = new float[inputSampleLength];
	for (unsigned int i = 0u; i < inputSampleLength; i++)
	{
		rootBuffer[i] = ((static_cast<float>(rawSample[i]) - 0.5f) / 32767.5f);
	}
	delete[] rawSample;

	float pitchFactor = BhMath::exp2(-(static_cast<float>(region.rootNote) - static_cast<float>(targetNote)) / 12.f);
	interpolatedBufferSize = static_cast<unsigned int>(inputSampleLength * 2 / pitchFactor);
	interpolatedBuffer = new float[interpolatedBufferSize];

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
		interpolatedBuffer[i] = rootBuffer[indexLower] * (1.f - t) + rootBuffer[indexUpper] * t;
	}

	delete[] rootBuffer;

	loopStart = static_cast<unsigned int>(region.loopStart / pitchFactor * 2);
	loopLength = static_cast<unsigned int>(region.loopLength / pitchFactor * 2);
}

gmSound::~gmSound()
{
	if (interpolatedBuffer != nullptr)
	{
		delete[] interpolatedBuffer;
		interpolatedBuffer = nullptr;
	}
}
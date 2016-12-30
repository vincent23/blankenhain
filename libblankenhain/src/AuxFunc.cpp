#include "AuxFunc.h"

#include <cmath>

// Auxiliary helper functions
namespace aux
{
	unsigned int frequencyToNearestLowerNote(float frequency)
	{
		return static_cast<unsigned int>(12.f * log2(frequency / 440.f)) + 69u;
	};

	float noteToFrequency(unsigned int note)
	{
		// via http://stackoverflow.com/questions/5779127/convert-frequency-to-keyboard-note
		return 440.f * exp((static_cast<int>(note) - 69)*log(2.f) / 12.f);
	}

	float linearToDecibel(float linear)
	{
		float db;

		if (linear != 0.0f)
			db = 20.0f * static_cast<float>(log10(linear));
		else
			db = -144.0f;  // effectively minus infinity

		return db;
	}

	float decibelToLinear(float db)
	{
		float linear;

		if (db < -144.0f)
			linear = 0.f;
		else
			linear = static_cast<float>(pow(10.f, db / 20.f));

		return linear;
	}

	float millisecToSamples(float millisec, float samplerate)
	{
		return (millisec / 1000.f) * samplerate;
	}

	float samplesToMillisec(float samples, float samplerate)
	{
		return (samples / samplerate) * 1000.f;
	}
}
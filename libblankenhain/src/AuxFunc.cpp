#include "AuxFunc.h"

#include <cmath>

// Auxiliary helper functions
namespace aux
{
	unsigned int frequencyToNearestLowerNote(float frequency)
	{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
		if (frequency < 0.f)
			throw ("frequency was negative value\n");
#endif
		return static_cast<unsigned int>(12.f * log2(frequency / 440.f)) + 68u;
	};

	float noteToFrequency(float note)
	{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
		if (note >= 128u)
			throw ("(midi)note was bigger than 127\n");
#endif
		// via http://stackoverflow.com/questions/5779127/convert-frequency-to-keyboard-note
		return 440.f * exp((note - 69.f)*log(2.f) / 12.f);
	}

	float calculateDetune(float frequency, float detune, unsigned int maxDetune)
	{
		unsigned int baseNote = aux::frequencyToNearestLowerNote(frequency);
		unsigned int countSemiTones = maxDetune;
		float freqLow = aux::noteToFrequency(baseNote == 0u ? 0u : baseNote - countSemiTones);
		float freqHigh = aux::noteToFrequency(baseNote == 127u ? 127u : baseNote  + countSemiTones);
		if (detune > 0.f)
			return (freqHigh - frequency) * detune / static_cast<float>(maxDetune) + frequency;
		else
			return (freqLow - frequency ) * detune * -1.f / static_cast<float>(maxDetune) + frequency;
	}

	float linearToDecibel(float linear)
	{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
		if (linear < 0.f)
			throw ("linear volume was negative value\n");
#endif
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
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
		if (millisec < 0.f)
			throw ("time in ms was negative value\n");
#endif
		return (millisec / 1000.f) * samplerate;
	}

	float samplesToMillisec(float samples, float samplerate)
	{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
		if (samples < 0.f)
			throw ("number of samples has negative value\n");
#endif
		return (samples / samplerate) * 1000.f;
	}
}
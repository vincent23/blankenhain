#pragma once
#include "Constants.h"
#include "Options.h"
#include "Sample.h"
#include "BhMath.h"

/*
 * Namespace for common conversions
 * of units and such
 *
 * Such as linearToDecibel or millisecToSample
 */
namespace aux
{
	inline unsigned int frequencyToNearestLowerNote(float frequency)
	{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
		if (frequency < 0.f)
			throw ("frequency was negative value\n");
#endif
		return static_cast<unsigned int>(12.f * BhMath::log2(frequency / 440.f)) + 68u;
	};

	inline float noteToFrequency(float note)
	{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
		if (note >= 128u)
			throw ("(midi)note was bigger than 127\n");
#endif
		// via http://stackoverflow.com/questions/5779127/convert-frequency-to-keyboard-note
		return 440.f * BhMath::exp2((note - 69.f) / 12.f);
	}

	inline float calculateDetune(float frequency, float detune, unsigned int maxDetune)
	{
		unsigned int baseNote = aux::frequencyToNearestLowerNote(frequency);
		unsigned int countSemiTones = maxDetune;
		float freqLow = aux::noteToFrequency(baseNote == 0u ? 0.f : static_cast<float>(baseNote - countSemiTones));
		float freqHigh = aux::noteToFrequency(baseNote == 127u ? 127.f : static_cast<float>(baseNote + countSemiTones));
		if (detune > 0.f)
			return (freqHigh - frequency) * detune / static_cast<float>(maxDetune) + frequency;
		else
			return (freqLow - frequency) * detune * -1.f / static_cast<float>(maxDetune) + frequency;
	}

	inline float linearToDecibel(float linear)
	{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
		if (linear < 0.f)
			throw ("linear volume was negative value\n");
#endif
		float db = 20.f * BhMath::log10(linear);

		//if (linear != 0.0f)
		//	db = 20.0f * static_cast<float>(BhMath::log10(linear));
		//else
		//	db = -144.0f;  // effectively minus infinity

		return db;
	}

	inline float decibelToLinear(float db)
	{
		float linear = BhMath::exp2(0.1660964047443681f * db);

		//if (db < -144.0f)
		//	linear = 0.f;
		//else
			// Use fastPow, tested it, its accurate enough for at least db -> lienar mapping
			// linear = BhMath::pow(10.f, db / 20.f);

		return linear;
	}

	inline float millisecToSamples(float millisec, float samplerate = constants::sampleRate)
	{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
		if (millisec < 0.f)
			throw ("time in ms was negative value\n");
#endif
		return (millisec / 1000.f) * samplerate;
	}

	inline float samplesToMillisec(float samples, float samplerate = constants::sampleRate)
	{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
		if (samples < 0.f)
			throw ("number of samples has negative value\n");
#endif
		return (samples / samplerate) * 1000.f;
	}

	inline float positionToTime(unsigned int samplePosition)
	{
		// perform this calculation in double precision
		double samplePositionDouble = static_cast<double>(samplePosition);
		double sampleRateDouble = static_cast<double>(constants::sampleRate);
		double timeInSeconds = samplePositionDouble / sampleRateDouble;
		return static_cast<float>(timeInSeconds);
	}
	template<typename T>
	inline T max(T a, T b)
	{
		return a > b ? a : b;
	}

	template<typename T>
	inline T min(T a, T b)
	{
		return a < b ? a : b;
	}

	inline void performPanning(Sample& sample, float const& panningBetweenMinusOneAndOne)
	{
		Sample panning(-panningBetweenMinusOneAndOne, panningBetweenMinusOneAndOne);
		panning = _mm_max_pd(panning.v, _mm_setzero_pd());
		Sample one(1);
		Sample boost(1.41253754462); // 3dB boost
		panning = ((one - panning) + panning * boost) * (one - panning.flippedChannels());
		sample *= panning;
	}

	inline Sample __vectorcall mixDryWet(Sample dry, Sample wet, float drywet)
	{
		return dry + (wet - dry) * Sample(drywet);
	}
}


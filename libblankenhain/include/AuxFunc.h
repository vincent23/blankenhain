#pragma once
#include "Constants.h"
#include "warnings.h"
#include "Sample.h"
/*
 * Namespace for common conversions 
 * of units and such
 *
 * Such as linearToDecibel or millisecToSample
 */
namespace aux
{
	float linearToDecibel(float linear);
	float decibelToLinear(float db);

	float millisecToSamples(float millisec, float samplerate = constants::sampleRate);
	float samplesToMillisec(float samples, float samplerate = constants::sampleRate);

	float noteToFrequency(float note);
	unsigned int frequencyToNearestLowerNote(float frequency);

	float calculateDetune(float frequency, float detune, unsigned int maxDetune = 1u);

	template<typename T>
	T max(T a, T b);

	template<typename T>
	T min(T a, T b);

	float positionToTime(unsigned int samplePosition);

	void performPanning(Sample& sample, float const& panning);
}


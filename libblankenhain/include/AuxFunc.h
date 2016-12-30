#pragma once
#include "Constants.h"
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

	float noteToFrequency(unsigned int note);
	unsigned int frequencyToNearestLowerNote(float frequency);
}


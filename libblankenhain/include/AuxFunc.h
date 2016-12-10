#pragma once

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

	float millisecToSamples(float millisec, float samplerate = 44100.f);
	float samplesToMillisec(float samples, float samplerate = 44100.f);
}


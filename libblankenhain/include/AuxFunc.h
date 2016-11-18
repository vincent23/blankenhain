#pragma once

namespace aux
{
	float linearToDecibel(float linear);
	float decibelToLinear(float db);

	float millisecToSamples(float millisec, float samplerate = 44100.f);
	float samplesToMillisec(float samples, float samplerate = 44100.f);
}


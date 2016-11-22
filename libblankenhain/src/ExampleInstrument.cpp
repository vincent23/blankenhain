#include "ExampleInstrument.h"

#include "ParameterBundle.h"
#include "FloatParameter.h"
#include "VoiceState.h"

#include <cmath>

ExampleInstrument::ExampleInstrument()
	: InstrumentBase(1, 4)
{
	getPointerToParameterBundle()->getParameter(0) = new FloatParameter(1.f, NormalizedRange(0.01f, 12.f, 1.f), "ratio", "");
}


void ExampleInstrument::processVoice(VoiceState& voice, unsigned int timeInSamples, Sample* buffer, unsigned int numberOfSamples)
{
	// assume sample rate = 44100
	float f_base = 440.f / 44100;
	float f = exp2((float(voice.key) - 69)/ 12) * f_base;
	float tau = 2.f * acos(-1.f);
	for (unsigned int sampleIndex = 0; sampleIndex < numberOfSamples; sampleIndex++) {
		if (voice.isOn) {
			unsigned int deltaT = (timeInSamples + sampleIndex) - voice.onTime;
			buffer[sampleIndex] = Sample(sin(tau * f * float(deltaT)));
		}
		else {
			// switch off note immediately
			voice.isSilent = true;
		}
	}
}
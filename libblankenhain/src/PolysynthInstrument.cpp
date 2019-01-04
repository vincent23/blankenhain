#include "PolysynthInstrument.h"
#include "InterpolatedValue.h"
#include "ParameterBundle.h"
#include "FloatParameter.h"
#include "VoiceState.h"

PolysynthInstrument::PolysynthInstrument()
	: InstrumentBase(9, 4)
{
	ParameterBundle& params = getParameterBundle();

	params.initParameter(0, new FloatParameter(50.f, NormalizedRange(0.1f, 1700.f, 0.3f), "attack", "ms"));
	params.initParameter(1, new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "hold", "ms"));
	params.initParameter(2, new FloatParameter(1.f, NormalizedRange(), "holdlevel", "ratio"));
	params.initParameter(3, new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "decay", "ms"));
	params.initParameter(4, new BoolParameter(false, "sustainBool"));
	params.initParameter(5, new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "sustain", "ms"));
	params.initParameter(6, new FloatParameter(1.0f, NormalizedRange(), "sustainLevel", "ratio"));
	params.initParameter(7, new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "release", "ms"));

	const BhString names[5] = { "sine", "saw", "square", "triangle" };
	params.initParameter(8, new OptionParameter(4u, names, "lfoWaveform", ""));
}

PolysynthInstrument::~PolysynthInstrument()
{
}

void PolysynthInstrument::processVoice(VoiceState& voice, unsigned int timeInSamples, Sample* buffer, unsigned int numberOfSamples)
{
	const float attack = interpolatedParameters.get(0);
	const float hold = interpolatedParameters.get(1);
	const float holdLevel = interpolatedParameters.get(2);
	const float decay = interpolatedParameters.get(3);
	const bool sustainOn = interpolatedParameters.get(4) > 0.5 ? true : false;
	const float sustainLevel = interpolatedParameters.get(6);
	const float sustain = interpolatedParameters.get(5);
	const float release = interpolatedParameters.get(7);
	const unsigned int oscMode = static_cast<unsigned int>(interpolatedParameters.get(8));

	// oscMode 0: Sine
	// oscMode 1: polyBLEP Sawtooth
	// oscMode 2: polyBLEP Square 
	// oscMode 3: polyBLEP Triangle

	this->osc.setFrequency(aux::noteToFrequency(static_cast<float>(voice.key)));

	this->osc.setMode(NaiveOscillator::NaiveOscillatorMode(oscMode));

	for (unsigned int sampleIndex = 0; sampleIndex < numberOfSamples; sampleIndex++) 
	{
		unsigned int deltaT = (timeInSamples + sampleIndex) - voice.onTime;
		buffer[sampleIndex] = Sample(this->osc.getSample(deltaT));
		performAHDSR<Sample>(buffer, voice, timeInSamples, sampleIndex, attack, release, hold, decay, sustain, sustainOn, sustainLevel, holdLevel);
	}
}
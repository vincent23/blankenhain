#include "ExampleInstrument.h"
#include "InterpolatedValue.h"
#include "ParameterBundle.h"
#include "FloatParameter.h"
#include "VoiceState.h"

ExampleInstrument::ExampleInstrument()
	: InstrumentBase(9, 4), currentSound(nullptr), pulse(0u)
{
	ParameterBundle& params = getParameterBundle();
	params.initParameter(0, new FloatParameter(5.f, NormalizedRange(0.f, 1700.f, 0.3f), "attack", "ms"));
	params.initParameter(1, new FloatParameter(100.f, NormalizedRange(0.f, 1700.f, 0.3f), "hold", "ms"));
	params.initParameter(2, new FloatParameter(1.f, NormalizedRange(), "holdlevel", "ratio"));
	params.initParameter(3, new FloatParameter(100.f, NormalizedRange(0.f, 1700.f, 0.3f), "decay", "ms"));
	params.initParameter(4, new FloatParameter(0.f, NormalizedRange(), "sustainBool", "bool"));
	params.initParameter(5, new FloatParameter(100.f, NormalizedRange(0.f, 1700.f, 0.3f), "sustain", "ms"));
	params.initParameter(6, new FloatParameter(1.0f, NormalizedRange(), "sustainLevel", "ratio"));
	params.initParameter(7, new FloatParameter(100.f, NormalizedRange(0.f, 1700.f, 0.3f), "release", "ms"));
	BhString modes[3] = { "Sine", "Noise", "Pulse" };
	params.initParameter(8, new OptionParameter(3u, modes, "mode", ""));
	osc.setMode(NaiveOscillator::NaiveOscillatorMode::OSCILLATOR_MODE_SINE);
}

void ExampleInstrument::processVoice(VoiceState& voice, unsigned int timeInSamples, Sample* buffer, unsigned int numberOfSamples)
{

	const float attack = interpolatedParameters.get(0u, 0u);
	const float hold = interpolatedParameters.get(1u);
	const float holdLevel = interpolatedParameters.get(2u);
	const float decay = interpolatedParameters.get(3u);
	const bool sustainOn = interpolatedParameters.get(4u) > 0.5 ? true : false;
	const float sustainLevel = interpolatedParameters.get(6u);
	const float sustain = interpolatedParameters.get(5u);
	const float release = interpolatedParameters.get(7u);
	const unsigned int mode = static_cast<unsigned int>(interpolatedParameters.get(8u));
	
	this->osc.setFrequency(aux::noteToFrequency(static_cast<float>(voice.key)));

	if (mode == 0u)
		currentSound = &osc;
	else if (mode == 1u)
		currentSound = &nosc;
	else
		currentSound = &pulse;

	for (unsigned int sampleIndex = 0; sampleIndex < numberOfSamples; sampleIndex++)
	{
		unsigned int deltaT = (timeInSamples + sampleIndex) - voice.onTime;
		buffer[sampleIndex] = Sample(this->currentSound->getSample(deltaT));
		performAHDSR<Sample>(buffer, voice, timeInSamples, sampleIndex, attack, release, hold, decay, sustain, sustainOn, sustainLevel, holdLevel);
	}
}

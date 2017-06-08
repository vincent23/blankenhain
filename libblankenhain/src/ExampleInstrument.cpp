#include "ExampleInstrument.h"
#include "InterpolatedValue.h"
#include "ParameterBundle.h"
#include "FloatParameter.h"
#include "VoiceState.h"

ExampleInstrument::ExampleInstrument()
	: InstrumentBase(9, 4), currentSound(nullptr)
{
	ParameterBundle& params = getParameterBundle();

	params.initParameter(0, new FloatParameter(50.f, NormalizedRange(1.f, 1700.f, 0.3f), "attack", "ms"));
	params.initParameter(1, new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "hold", "ms"));
	params.initParameter(2, new FloatParameter(1.f, NormalizedRange(), "holdlevel", "ratio"));
	params.initParameter(3, new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "decay", "ms"));
	params.initParameter(4, new FloatParameter(0.f, NormalizedRange(), "sustainBool", "bool"));
	params.initParameter(5, new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "sustain", "ms"));
	params.initParameter(6, new FloatParameter(1.0f, NormalizedRange(), "sustainLevel", "ratio"));
	params.initParameter(7, new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "release", "ms"));
	params.initParameter(8, new FloatParameter(0.f, NormalizedRange(), "mode", ""));
	osc.setMode(NaiveOscillator::NaiveOscillatorMode::OSCILLATOR_MODE_SINE);
}

void ExampleInstrument::processVoice(VoiceState& voice, unsigned int timeInSamples, Sample* buffer, unsigned int numberOfSamples)
{

	float attack = interpolatedParameters.get(0);
	float hold = interpolatedParameters.get(1);
	float holdLevel = interpolatedParameters.get(2);
	float decay = interpolatedParameters.get(3);
	bool sustainOn = interpolatedParameters.get(4) > 0.5 ? true : false;
	float sustainLevel = interpolatedParameters.get(6);
	float sustain = interpolatedParameters.get(5);
	float release = interpolatedParameters.get(7);
	unsigned int mode = static_cast<unsigned int>(interpolatedParameters.get(8));
	
	this->osc.setFrequency(aux::noteToFrequency(static_cast<float>(voice.key)));

	if (mode == 0u)
		currentSound = &osc;
	else
		currentSound = &nosc;

	for (unsigned int sampleIndex = 0; sampleIndex < numberOfSamples; sampleIndex++)
	{
		unsigned int deltaT = (timeInSamples + sampleIndex) - voice.onTime;
		buffer[sampleIndex] = Sample(this->currentSound->getSample(deltaT));
		performAHDSR<Sample>(buffer, voice, timeInSamples, sampleIndex, attack, release, hold, decay, sustain, sustainOn, sustainLevel, holdLevel);
	}
}
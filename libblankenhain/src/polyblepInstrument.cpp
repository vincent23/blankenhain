#include "polyblepInstrument.h"
#include "InterpolatedValue.h"
#include "ParameterBundle.h"
#include "FloatParameter.h"
#include "VoiceState.h"

polyblepInstrument::polyblepInstrument()
	: InstrumentBase(9, 4)
{
	ParameterBundle& params = getParameterBundle();

	params.getParameter(0) = new FloatParameter(50.f, NormalizedRange(0.1f, 1700.f, 0.3f), "attack", "ms");
	params.getParameter(1) = new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "hold", "ms");
	params.getParameter(2) = new FloatParameter(1.f, NormalizedRange(), "holdlevel", "ratio");
	params.getParameter(3) = new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "decay", "ms");
	params.getParameter(4) = new FloatParameter(0.f, NormalizedRange(), "sustainBool", "bool");
	params.getParameter(5) = new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "sustain", "ms");
	params.getParameter(6) = new FloatParameter(1.0f, NormalizedRange(), "sustainLevel", "ratio");
	params.getParameter(7) = new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "release", "ms");
	params.getParameter(8) = new FloatParameter(0.f, NormalizedRange(0.f, 3.9f), "osc", "");

}

polyblepInstrument::~polyblepInstrument()
{
}

void polyblepInstrument::processVoice(VoiceState& voice, unsigned int timeInSamples, Sample* buffer, unsigned int numberOfSamples)
{

	float attack = interpolatedParameters.get(0);
	float hold = interpolatedParameters.get(1);
	float holdLevel = interpolatedParameters.get(2);
	float decay = interpolatedParameters.get(3);
	bool sustainOn = interpolatedParameters.get(4) > 0.5 ? true : false;
	float sustainLevel = interpolatedParameters.get(6);
	float sustain = interpolatedParameters.get(5);
	float release = interpolatedParameters.get(7);
	unsigned int oscMode = static_cast<unsigned int>(interpolatedParameters.get(8));


	// oscMode 1: polyBLEP Sawtooth
	// oscMode 2: polyBLEP Square (broken)
	// oscMode 3: polyBLEP Triangle (broken)

	this->osc.setFrequency(aux::noteToFrequency(static_cast<float>(voice.key)));

	this->osc.setMode(NaiveOscillator::NaiveOscillatorMode(oscMode));

	for (unsigned int sampleIndex = 0; sampleIndex < numberOfSamples; sampleIndex++) 
	{
		unsigned int deltaT = (timeInSamples + sampleIndex) - voice.onTime;
		buffer[sampleIndex] = Sample(this->osc.getSample(deltaT));
		performAHDSR<Sample>(buffer, voice, timeInSamples, sampleIndex, attack, release, hold, decay, sustain, sustainOn, sustainLevel, holdLevel);
	}
}
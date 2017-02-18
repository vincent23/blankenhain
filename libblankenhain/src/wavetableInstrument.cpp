#include "wavetableInstrument.h"
#include "InterpolatedValue.h"
#include "ParameterBundle.h"
#include "FloatParameter.h"
#include "VoiceState.h"

wavetableInstrument::wavetableInstrument()
	: InstrumentBase(9, 4),
	tri_osc(AdditiveTriangleWaveOscillator()),
	sq_osc(AdditiveSquareWaveOscillator()),
	sawt_osc(AdditiveSawtoothWaveOscillator()),
	noise_osc1(NoiseOscillator()),
	noise_osc2(NoiseOscillator(), 150.f)
{
	ParameterBundle* params = getPointerToParameterBundle();

	params->getParameter(0) = new FloatParameter(50.f, NormalizedRange(0.1f, 1700.f, 0.3f), "attack", "ms");
	params->getParameter(1) = new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "hold", "ms");
	params->getParameter(2) = new FloatParameter(1.f, NormalizedRange(), "holdlevel", "ratio");
	params->getParameter(3) = new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "decay", "ms");
	params->getParameter(4) = new FloatParameter(0.f, NormalizedRange(), "sustainBool", "bool");
	params->getParameter(5) = new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "sustain", "ms");
	params->getParameter(6) = new FloatParameter(1.0f, NormalizedRange(), "sustainLevel", "ratio");
	params->getParameter(7) = new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "release", "ms");
	params->getParameter(8) = new FloatParameter(0.f, NormalizedRange(0.f, 4.9f), "osc", "");

}

wavetableInstrument::~wavetableInstrument()
{
}

void wavetableInstrument::processVoice(VoiceState& voice, unsigned int timeInSamples, Sample* buffer, unsigned int numberOfSamples)
{

	float attack = getInterpolatedParameter(0).get();
	float hold = getInterpolatedParameter(1).get();
	float holdLevel = getInterpolatedParameter(2).get();
	float decay = getInterpolatedParameter(3).get();
	bool sustainOn = getInterpolatedParameter(4).get() > 0.5 ? true : false;
	float sustainLevel = getInterpolatedParameter(6).get();
	float sustain = getInterpolatedParameter(5).get();
	float release = getInterpolatedParameter(7).get();
	unsigned int oscMode = static_cast<unsigned int>(getInterpolatedParameter(8).get());

	// oscMode 0: WaveTable Square
	// oscMode 1: WaveTable Triangle
	// oscMode 2: WaveTable Sawtooth
	// oscMode 4: WaveTable Noise HQ
	// oscMode 5: WaveTable Noise LQ

	this->sq_osc.setFrequency(aux::noteToFrequency(static_cast<float>(voice.key)));
	this->tri_osc.setFrequency(aux::noteToFrequency(static_cast<float>(voice.key)));
	this->noise_osc1.setFrequency(aux::noteToFrequency(static_cast<float>(voice.key)));
	this->noise_osc2.setFrequency(aux::noteToFrequency(static_cast<float>(voice.key)));
	this->sawt_osc.setFrequency(aux::noteToFrequency(static_cast<float>(voice.key)));

	for (unsigned int sampleIndex = 0; sampleIndex < numberOfSamples; sampleIndex++) {
		unsigned int deltaT = (timeInSamples + sampleIndex) - voice.onTime;

		if (oscMode == 0u)
			buffer[sampleIndex] = Sample(this->sq_osc.getSample(deltaT));
		else if (oscMode == 1u)
		{
			buffer[sampleIndex] = Sample(this->tri_osc.getSample(deltaT));
		}
		else if (oscMode == 2u)
		{
			buffer[sampleIndex] = Sample(this->sawt_osc.getSample(deltaT));
		}
		else if (oscMode == 3u)
		{
			buffer[sampleIndex] = Sample(this->noise_osc1.getSample(deltaT));
		}
		else 
		{
			buffer[sampleIndex] = Sample(this->noise_osc1.getSample(deltaT));
		}
		performAHDSR<Sample>(buffer, voice, timeInSamples, sampleIndex, attack, release, hold, decay, sustain, sustainOn, sustainLevel, holdLevel);
	}
}
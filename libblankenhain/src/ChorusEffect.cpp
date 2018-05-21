#include "ChorusEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"
#include <algorithm>

ChorusEffect::ChorusEffect()
	: EffectBase(12,true)
	//, delayLine(size_t(aux::millisecToSamples(2502u)))
	, delayLine(1 << 17)
{
	wasPaniced = false;
	ParameterBundle& params = getParameterBundle();
	params.initParameter(0, new FloatParameter(0.f, NormalizedRange(0.f, 1.f), "lfoAmount", ""));
	//params.initParameter(1, new FloatParameter(0.0001f, NormalizedRange(0.0001f, 1.f, 0.2f), "width", "%"));
	params.initParameter(2, new FloatParameter(15.f, NormalizedRange(0.5f, 24.f, 0.2f), "delay", "ms"));
	params.initParameter(3, new FloatParameter(0.0f, NormalizedRange(0.0f, 1.f), "feedback", "%"));
	params.initParameter(4, new FloatParameter(0.f, NormalizedRange(), "drywet", ""));
	params.initParameter(5, new BoolParameter(false, "PANIC!"));
	BhString names[5] = { "sine", "saw", "square", "triangle", "noise" };
	params.initParameter(6, new OptionParameter(4u, names, "lfoWaveform", ""));
	params.initParameter(7, new FloatParameter(0.f, NormalizedRange(0.f, 2.f * constants::pi), "lfoPhase", ""));
	params.initParameter(8, new FloatParameter(0.f, NormalizedRange(-50.f, 50.f), "pan", ""));
	params.initParameter(9, new FloatParameter(5.f, NormalizedRange(0.005f, 1.f, 0.3f), "lfoSpeed", "Hz"));
	params.initParameter(1, new BoolParameter(false, "pseudoStereo"));
	params.initParameter(10, new BoolParameter(false, "tempoSyncOn"));
	float multiplierValues[7] = { 0.0625, 0.125, 0.25, 0.5, 1., 2., 4. };
	params.initParameter(11, new DiscreteParameter(7u, "multiplier", "", multiplierValues, 4u));
}

void ChorusEffect::process(Sample* buffer, size_t numberOfSamples, size_t currentTime)
{
	const float delay = interpolatedParameters.get(2);

	const float feedback = interpolatedParameters.get(3);
	const float drywet = interpolatedParameters.get(4);
	const bool panicButton = interpolatedParameters.get(5) == 1.f;
	const bool stereoButton = interpolatedParameters.get(1) == 1.f;
	const float pan = interpolatedParameters.get(8);
	
	const float lfoAmount = interpolatedParameters.get(0);
	const float lfoWaveform = interpolatedParameters.get(6);
	this->lfo.setMode(NaiveOscillator::NaiveOscillatorMode(static_cast<unsigned int>(lfoWaveform)));
	const float lfoPhase = interpolatedParameters.get(7);
	const float lfoSpeed = interpolatedParameters.get(9);

	const bool tempoSync = interpolatedParameters.get(10) == 1.f;
	float tempoSyncVal = interpolatedParameters.get(11);

	float lfofreq = lfoSpeed;
	if (tempoSync)
	{
		if (tempoSyncVal < 0.125f)
			tempoSyncVal = 0.0625;
		else if (tempoSyncVal < 0.25)
			tempoSyncVal = 0.125f;
		else if (tempoSyncVal < 0.5)
			tempoSyncVal = 0.25;
		else if (tempoSyncVal < 1.f)
			tempoSyncVal = 0.5;
		else if (tempoSyncVal < 1.5f)
			tempoSyncVal = 1.f;
		else if (tempoSyncVal >= 1.5)
			tempoSyncVal = 2.f;

		float quarterNoteLength = (60.f /*seconds in a minute*/ * tempoSyncVal) / tempodata.bpm;
		float sixteenthNoteLength = quarterNoteLength / 4.f;
		float wholeBeatLength = sixteenthNoteLength * 16.f;
		lfofreq = 1.f / wholeBeatLength;
	}


	this->lfo.setFrequency(lfofreq);

	const unsigned int delayLength = static_cast<unsigned int>(aux::millisecToSamples(delay));

	// Panic
	if (panicButton && !wasPaniced)
	{
		wasPaniced = true;
		delayLine.reset();
	}
	else if (wasPaniced && !panicButton)
	{
		wasPaniced = false;
	}


	
	for (unsigned int i = 0; i < numberOfSamples; i++)
	{
		float lfoValue = (this->lfo.getNextSample(lfoPhase)) * lfoAmount;

		float currentSweepPosition = lfoValue * delayLength + delayLength;
			
		Sample inval = buffer[i];
		Sample outval = delayLine.get((currentSweepPosition));
		delayLine.push(Sample(feedback) * outval + inval);

		// Pan
		aux::performPanning(outval, pan * 0.02f);

		if (stereoButton)
			outval.replaceLeftChannel(-outval);
		//Ghetto Stereo by PhaseShift, maybe TODO use seperate DelayLines for L & R

		buffer[i] = aux::mixDryWet(inval, outval, drywet);
	}
	
}
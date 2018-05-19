#include "ChorusEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"
#include <algorithm>

ChorusEffect::ChorusEffect()
	: EffectBase(11)
	//, delayLine(size_t(aux::millisecToSamples(2502u)))
	, delayLine(1 << 17)
{
	wasPaniced = false;
	ParameterBundle& params = getParameterBundle();
	params.initParameter(0, new FloatParameter(0.f, NormalizedRange(0.f, 1.f), "lfoAmount", ""));
	params.initParameter(1, new FloatParameter(0.0001f, NormalizedRange(0.0001f, 1.f, 0.2f), "width", "%"));
	params.initParameter(2, new FloatParameter(15.f, NormalizedRange(1.f, 50.f, 0.2f), "delay", "ms"));
	params.initParameter(3, new FloatParameter(0.0f, NormalizedRange(0.0f, 1.f), "feedback", "%"));
	params.initParameter(4, new FloatParameter(0.f, NormalizedRange(), "drywet", ""));
	params.initParameter(5, new BoolParameter(false, "PANIC!"));
	BhString names[5] = { "sine", "saw", "square", "triangle", "noise" };
	params.initParameter(6, new OptionParameter(4u, names, "lfoWaveform", ""));
	params.initParameter(7, new FloatParameter(0.f, NormalizedRange(0.f, 2.f * constants::pi), "lfoPhase", ""));
	params.initParameter(8, new FloatParameter(0.f, NormalizedRange(-50.f, 50.f), "pan", ""));
	params.initParameter(9, new FloatParameter(5.f, NormalizedRange(0.01f, 100.f, 0.3f), "lfoSpeed", "ms"));
	params.initParameter(10, new BoolParameter(false, "pseudoStereo"));
}

void ChorusEffect::process(Sample* buffer, size_t numberOfSamples, size_t currentTime)
{

	float delay = interpolatedParameters.get(2);
	float width = interpolatedParameters.get(1) * delay;
	float feedback = interpolatedParameters.get(3);
	float drywet = interpolatedParameters.get(4);
	bool panicButton = interpolatedParameters.get(5) == 1.f;
	bool stereoButton = interpolatedParameters.get(10) == 1.f;
	float pan = interpolatedParameters.get(8);

	float lfoAmount = interpolatedParameters.get(0);
	float lfoWaveform = interpolatedParameters.get(6);
	this->lfo.setMode(NaiveOscillator::NaiveOscillatorMode(static_cast<unsigned int>(lfoWaveform)));
	float lfoPhase = interpolatedParameters.get(7);
	float lfoSpeed = interpolatedParameters.get(9);
	this->lfo.setFrequency(lfoSpeed);

	//delayLine.setSize(static_cast<size_t>(aux::millisecToSamples(delay)));
	unsigned int delayLength = static_cast<unsigned int>(aux::millisecToSamples(delay));

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


	/*
	for (unsigned int i = 0; i < numberOfSamples; i++)
	{
		float lfoValue = (this->lfo.getNextSample(lfoPhase) * .5f + 5.f) * lfoAmount;
		size_t const& position = delayLine.getCurrentIteratorInDelayline();
		float currentSweepPosition = BhMath::fmod(lfoValue * aux::millisecToSamples(width) + static_cast<float>(position), static_cast<float>(delayLine.getSize()));
		if (currentSweepPosition < 0.f)
		{
			currentSweepPosition += static_cast<float>(delayLine.getSize());
		}
		Sample inval = buffer[i];


		// TODO: Linear Interpolation
		Sample outval = delayLine.get(static_cast<int>(currentSweepPosition));

		delayLine.push(Sample(feedback) * outval + inval);


		// Pan
		aux::performPanning(outval, pan * 0.02f);
		if (stereoButton)
			outval.replaceLeftChannel(-outval);
		//Ghetto Stereo by PhaseShift, maybe TODO use seperate DelayLines for L & R
		buffer[i] = aux::mixDryWet(inval, outval, drywet);
	}
	*/
}
#include "ChorusEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"
#include <algorithm>

ChorusEffect::ChorusEffect() : EffectBase(11), delayLine(size_t(aux::millisecToSamples(2502u)))
{
	wasPaniced = false;
	ParameterBundle* params = getPointerToParameterBundle();
	params->getParameter(0) = new FloatParameter(0.f, NormalizedRange(0.f, 1.f), "lfoAmount", "");
	(params->getParameter(1)) = new FloatParameter(0.0001f, NormalizedRange(0.0001f, 1.f, 0.2f), "width", "%");
	(params->getParameter(2)) = new FloatParameter(15.f, NormalizedRange(1.f, 50.f, 0.2f), "delay", "ms");
	(params->getParameter(3)) = new FloatParameter(0.0f, NormalizedRange(0.0f, 1.f), "feedback", "%");
	(params->getParameter(4)) = new FloatParameter(0.f, NormalizedRange(), "drywet", "");
	(params->getParameter(5)) = new BoolParameter(false, "PANIC!");
	BhString names[5] = { "sine", "saw", "square", "triangle", "noise" };
	params->getParameter(6) = new OptionParameter(4u, names, "lfoWaveform", "");
	params->getParameter(7) = new FloatParameter(0.f, NormalizedRange(0.f, 2.f * constants::pi), "lfoPhase", "");
	(params->getParameter(8)) = new FloatParameter(0.f, NormalizedRange(-50.f, 50.f), "pan", "");
	params->getParameter(9) = new FloatParameter(5.f, NormalizedRange(0.01f, 100.f, 0.3f), "lfoSpeed", "ms");
	params->getParameter(10) = new BoolParameter(false, "pseudoStereo");
}

void ChorusEffect::process(Sample* buffer, size_t numberOfSamples, size_t currentTime)
{

	float delay = getInterpolatedParameter(2).get();
	float width = getInterpolatedParameter(1).get() * delay;
	float feedback = getInterpolatedParameter(3).get();
	float drywet = getInterpolatedParameter(4).get();
	bool panicButton = getInterpolatedParameter(5).get() == 1.f;
	bool stereoButton = getInterpolatedParameter(10).get() == 1.f;
	float pan = getInterpolatedParameter(8).get();

	float lfoAmount = getInterpolatedParameter(0).get();
	float lfoWaveform = getInterpolatedParameter(6).get();
	this->lfo.setMode(NaiveOscillator::NaiveOscillatorMode(static_cast<unsigned int>(lfoWaveform)));
	float lfoPhase = getInterpolatedParameter(7).get();
	float lfoSpeed = getInterpolatedParameter(9).get();
	this->lfo.setFrequency(lfoSpeed);

	delayLine.setSize(static_cast<size_t>(aux::millisecToSamples(delay)));

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

}
#include "ChorusEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"
#include <algorithm>

ChorusEffect::ChorusEffect() : EffectBase(9), delayLine(size_t(aux::millisecToSamples(2502u)))
{
	wasPaniced = false;
	ParameterBundle* params = getPointerToParameterBundle();
	params->getParameter(0) = new FloatParameter(0.f, NormalizedRange(-1.f, 1.f), "lfoAmount", "");
	(params->getParameter(1)) = new FloatParameter(2.f, NormalizedRange(1.f, 25.f), "width", "ms");
	(params->getParameter(2)) = new FloatParameter(15.f, NormalizedRange(1.f, 100.f, 0.5f), "delay", "ms");
	(params->getParameter(3)) = new FloatParameter(0.5f, NormalizedRange(0.0f, 1.f), "feedback", "%");
	(params->getParameter(4)) = new FloatParameter(0.f, NormalizedRange(), "drywet", "");
	(params->getParameter(5)) = new BoolParameter(false, "PANIC!");
	BhString names[5] = { "sine", "saw", "square", "triangle", "noise" };
	params->getParameter(6) = new OptionParameter(4u, names, "lfoWaveform", "");
	params->getParameter(7) = new FloatParameter(0.f, NormalizedRange(0.f, 2.f * constants::pi), "lfoPhase", "");
	(params->getParameter(8)) = new FloatParameter(0.f, NormalizedRange(-50.f, 50.f), "pan", "");
}

void ChorusEffect::process(Sample* buffer, size_t numberOfSamples)
{
	float width = getInterpolatedParameter(1).get();
	float delay = getInterpolatedParameter(2).get();
	float feedback = getInterpolatedParameter(3).get();
	float drywet = getInterpolatedParameter(4).get();
	bool panicButton = static_cast<bool>(getInterpolatedParameter(5).get());
	float pan = getInterpolatedParameter(8).get();

	float lfoAmount = getInterpolatedParameter(0).get();
	float lfoWaveform = getInterpolatedParameter(6).get();
	this->lfo.setMode(NaiveOscillator::NaiveOscillatorMode(static_cast<unsigned int>(lfoWaveform)));
	float lfoPhase = getInterpolatedParameter(7).get();
	float lfoSpeed = getInterpolatedParameter(11).get();
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
		float lfoValue = this->lfo.getNextSample(lfoPhase) * lfoAmount;
		size_t const& position = delayLine.getCurrentIteratorInDelayline();
		float currentSweepPosition = BhMath::fmod(lfoValue * aux::millisecToSamples(width) + static_cast<float>(position), static_cast<float>(delayLine.getSize()));

		Sample inval = buffer[i];
		//delayLine.push(inval);


		// TODO: Linear Interpolation
		Sample outval = delayLine.get(static_cast<int>(currentSweepPosition));

		// drywet
		if (drywet > 0.5)
		{
			buffer[i] *= Sample((1 - drywet) * 2.f);
		}
		else
		{
			outval *= Sample(drywet * 2.f);
		}

		// Pan
		// TODO: Without load and store
		alignas(16) double lr[2];
		outval.store_aligned(lr);
		lr[1] = lr[0] * (1.f - (0.0f < pan ? pan : 0.0f) * 0.02f);
		lr[0] = lr[1] * (1.f + (0.0f < pan ? 0.0f : pan) * 0.02f) * -1.f ; //Ghetto Stereo by PhaseShift, maybe TODO use seperate DelayLines for L & R
		outval = Sample::load_aligned(lr);

		buffer[i] += outval;
		delayLine.push(Sample(feedback) * buffer[i] + inval);
	}

	nextSample(numberOfSamples);

}
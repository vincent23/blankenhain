#include "DelayEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"

DelayEffect::DelayEffect() : EffectBase(7, true), delayLine(size_t(aux::millisecToSamples(2502u)))
{
	wasPaniced = false;
	ParameterBundle* params = getPointerToParameterBundle();
	(params->getParameter(0)) = new FloatParameter(0.f, NormalizedRange(-50.f, 50.f), "pan", "");
	(params->getParameter(1)) = new FloatParameter(100.f, NormalizedRange(1.f, 2500.f, 0.3), "length", "ms");
	(params->getParameter(2)) = new FloatParameter(0.f, NormalizedRange(0.f, 1.5f), "feedback", "");
	(params->getParameter(3)) = new FloatParameter(1.f, NormalizedRange(), "drywet", "");
	(params->getParameter(4)) = new BoolParameter(false, "PANIC!");
	float multiplierValues[7] = { 0.0625, 0.125, 0.25, 0.5, 1., 2., 4. };
	params->getParameter(5) = new DiscreteParameter(7u, "beatMultiplier", "", multiplierValues);
	params->getParameter(6) = new BoolParameter(false, "lfoTemposync");

}

void DelayEffect::process(Sample* buffer, size_t numberOfSamples)
{
	InterpolatedValue<float>& pan = getInterpolatedParameter(0);
	InterpolatedValue<float>& length = getInterpolatedParameter(1);
	InterpolatedValue<float>& feedback = getInterpolatedParameter(2);
	InterpolatedValue<float>& drywet = getInterpolatedParameter(3);
	bool panicButton = static_cast<bool>(getInterpolatedParameter(4).get());
	bool tempoSync = static_cast<bool>(getInterpolatedParameter(6).get());

	if (!tempoSync)
		delayLine.setSize(static_cast<size_t>(aux::millisecToSamples(length.get())));
	else
	{
		float beatMultiplier = getInterpolatedParameter(5).get();

		if (beatMultiplier < 0.125f)
			beatMultiplier = 0.0625;
		else if (beatMultiplier < 0.25)
			beatMultiplier = 0.125f;
		else if (beatMultiplier < 0.5)
			beatMultiplier = 0.25;
		else if (beatMultiplier < 1.f)
			beatMultiplier = 0.5;
		else if (beatMultiplier < 1.9f)
			beatMultiplier = 1.f;
		else if (beatMultiplier < 3.5f)
			beatMultiplier = 2.f;
		else
			beatMultiplier = 4.f;

		float quarterNoteLength = (60.f /*seconds in a minute*/ * beatMultiplier) / tempodata.bpm;
		float sixteenthNoteLength = quarterNoteLength / 4.f;
		float wholeBeatLength = sixteenthNoteLength * 16.f;

		delayLine.setSize(static_cast<size_t>(aux::millisecToSamples(quarterNoteLength * 1000.f/*sec to ms*/)));
	}

	if (panicButton && !wasPaniced)
	{
		wasPaniced = true;
		delayLine.reset();
	}
	else if (wasPaniced && !panicButton)
	{
		wasPaniced = false;
	}

	for (size_t i = 0; i < numberOfSamples; i++)
	{
		double avg_ = buffer[i].avgValue();
		Sample original = buffer[i];
		Sample line = delayLine.get();

		if (drywet.get() > 0.5)
		{
			original *= Sample((1 - drywet.get()) * 2.f);
		}
		else line *= Sample(drywet.get() * 2.f);

		// Pan
		alignas(16) double lr[2];
		line.store_aligned(lr);
		lr[0] = lr[0] * (1.f - (0.0f < pan.get() ? pan.get() : 0.0f) * 0.02f);
		lr[1] = lr[1] * (1.f + (0.0f < pan.get() ? 0.0f : pan.get()) * 0.02f);
		line = Sample::load_aligned(lr);

		buffer[i] = original + line;

		delayLine.push(delayLine.get() * Sample(feedback.get()) + Sample(avg_));
		nextSample();
	}
}

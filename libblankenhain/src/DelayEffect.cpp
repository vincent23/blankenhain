#include "DelayEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"

DelayEffect::DelayEffect() : EffectBase(5), delayLine(size_t(aux::millisecToSamples(2502u)))
{
	wasPaniced = false;
	ParameterBundle* params = getPointerToParameterBundle();
	(params->getParameter(0)) = new FloatParameter(0.f, NormalizedRange(-50.f, 50.f), "pan", "");
	(params->getParameter(1)) = new FloatParameter(100.f, NormalizedRange(1.f, 2500.f), "length", "ms");
	(params->getParameter(2)) = new FloatParameter(0.f, NormalizedRange(0.f, 1.5f), "feedback", "");
	(params->getParameter(3)) = new FloatParameter(1.f, NormalizedRange(), "drywet", "");
	(params->getParameter(4)) = new BoolParameter(false, "PANIC!");
}

void DelayEffect::process(Sample* buffer, size_t numberOfSamples)
{
	InterpolatedValue<float>& pan = getInterpolatedParameter(0);
	InterpolatedValue<float>& length = getInterpolatedParameter(1);
	InterpolatedValue<float>& feedback = getInterpolatedParameter(2);
	InterpolatedValue<float>& drywet = getInterpolatedParameter(3);
	bool panicButton = static_cast<bool>(getInterpolatedParameter(4).get());

	delayLine.setSize(static_cast<size_t>(aux::millisecToSamples(length.get())));

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
		lr[0] = lr[0] * (1.f - aux::max(0.0f, pan.get()));
		lr[1] = lr[1] * (1.f + aux::min(0.0f, pan.get()));
		line = Sample::load_aligned(lr);

		buffer[i] = original + line;

		delayLine.push(delayLine.get() * Sample(feedback.get()) + Sample(avg_));
		nextSample();
	}
}
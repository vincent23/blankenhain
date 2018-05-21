#include "DelayEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"

DelayEffect::DelayEffect()
	: EffectBase(7, true)
	//, delayLine(size_t(aux::millisecToSamples(2502u)))
	, delayLine(1 << 17)
{
	wasPaniced = false;
	ParameterBundle& params = getParameterBundle();
	params.initParameter(0, new FloatParameter(0.f, NormalizedRange(-50.f, 50.f), "pan", ""));
	params.initParameter(1, new FloatParameter(100.f, NormalizedRange(1.f, 2500.f, 0.3f), "length", "ms"));
	params.initParameter(2, new FloatParameter(0.f, NormalizedRange(0.f, 1.5f), "feedback", ""));
	params.initParameter(3, new FloatParameter(0.5f, NormalizedRange(), "drywet", ""));
	params.initParameter(4, new BoolParameter(false, "PANIC!"));
	float multiplierValues[7] = { 0.0625, 0.125, 0.25, 0.5, 1., 2., 4. };
	params.initParameter(5, new DiscreteParameter(7u, "beatMultiplier", "", multiplierValues, 4u));
	params.initParameter(6, new BoolParameter(false, "lfoTemposync"));

}

void DelayEffect::process(Sample* buffer, size_t numberOfSamples, size_t currentTime)
{
	const float pan = interpolatedParameters.get(0);
	const float length = interpolatedParameters.get(1);
	const float feedback = interpolatedParameters.get(2);
	const float drywet = interpolatedParameters.get(3);
	const bool panicButton = interpolatedParameters.get(4) == 1.f;
	const bool tempoSync = interpolatedParameters.get(6) == 1.f;


	float beatMultiplier = interpolatedParameters.get(5);

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


	float delayLength;
	if (!tempoSync)
	{
		//delayLine.setSize(static_cast<size_t>(aux::millisecToSamples(length)));
		delayLength = aux::millisecToSamples(length * beatMultiplier);
	}
	else
	{
		const float quarterNoteLength = (60.f /*seconds in a minute*/ * beatMultiplier) / tempodata.bpm;
		const float sixteenthNoteLength = quarterNoteLength / 4.f;
		const float wholeBeatLength = sixteenthNoteLength * 16.f;

		//delayLine.setSize(static_cast<size_t>(aux::millisecToSamples(quarterNoteLength * 1000.f/*sec to ms*/)));
		delayLength = aux::millisecToSamples(quarterNoteLength * 1000.f /* sec to ms */);
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
		Sample original = buffer[i];
		Sample line = delayLine.getInterpolated(delayLength);
		aux::performPanning(line, pan * 0.02f); // Pan
		buffer[i] = aux::mixDryWet(original, line, drywet);
		delayLine.push(line * Sample(feedback) + original);
	}
}

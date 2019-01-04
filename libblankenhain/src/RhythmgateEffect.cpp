#include "RhythmgateEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"
#include "BhMath.h"

RhythmgateEffect::RhythmgateEffect() : EffectBase(21, true)
{
	// 16 levels,
	// attack release
	ParameterBundle& params = getParameterBundle();

	for (unsigned int i = 0u; i < 16u; i++)
	{
		BhString name;
#ifndef _VC_NODEFAULTLIB
		name = "gate" + std::to_string(i);
#else
		name = nullptr;
#endif
		params.initParameter(i, new FloatParameter(1.f, NormalizedRange(), name, ""));
	}
	params.initParameter(16u, new FloatParameter(0.001f, NormalizedRange(0.001f, .999f), "attack", ""));
	params.initParameter(17u, new FloatParameter(0.001f, NormalizedRange(0.001f, .999f), "release", ""));
	float multiplierValues[7] = { 0.0625, 0.125, 0.25, 0.5, 1., 2., 4. };
	params.initParameter(18u, new DiscreteParameter(7u, "beatMultiplier", "", multiplierValues, 4u));
	params.initParameter(19u, new FloatParameter(0.f, NormalizedRange(0.f, 6000.f, 0.37f), "offset", "ms"));
	params.initParameter(20u, new FloatParameter(1.f, NormalizedRange(0.f, 1.f), "DryWet", ""));
}

void RhythmgateEffect::process(Sample* buffer, size_t numberOfSamples, size_t currentTime)
{
	float params[16];
	for (unsigned int i = 0u; i < 16u; i++)
	{
		params[i] = interpolatedParameters.get(i);
	}

	float attack = interpolatedParameters.get(16u);
	float release = interpolatedParameters.get(17u);

	float multiplier = interpolatedParameters.get(18u);
	float offset = interpolatedParameters.get(19u);
	float drywet = interpolatedParameters.get(20u);

	for (size_t bufferIteration = 0; bufferIteration < numberOfSamples; bufferIteration++)
	{
		float quarterNoteLength = (60.f /*seconds in a minute*/ * multiplier) / tempodata.bpm; // in seconds
		float sixteenthNoteLength = quarterNoteLength / 4.f;
		float wholeBeatLength = sixteenthNoteLength * 16.f;
		float currentSecond = static_cast<float>(tempodata.position) / constants::sampleRate;

		currentSecond += offset / 1000.f;

		float currentPartialBeatInSeconds = BhMath::fmod(currentSecond, wholeBeatLength);
		float currentPartialSixteenthInSeconds = BhMath::fmod(currentSecond, sixteenthNoteLength);
		float currentPartialSixteenthPercent = currentPartialSixteenthInSeconds / sixteenthNoteLength;
		unsigned int whichSixteenthAreWeIn = static_cast<unsigned int>(currentPartialBeatInSeconds / sixteenthNoteLength);
		unsigned int lastSixteenth = whichSixteenthAreWeIn == 0 ? 15 : whichSixteenthAreWeIn - 1u;

		float last = params[lastSixteenth];
		float current = params[whichSixteenthAreWeIn];

		float cmult;
		if (current < last  && currentPartialSixteenthPercent < release)
		{
			cmult = last + (currentPartialSixteenthPercent / release) * (current - last);
		}
		else if (current > last && currentPartialSixteenthPercent < attack)
		{
			cmult = last + (currentPartialSixteenthPercent / attack) * (current - last);
		}
		else
		{
			cmult = params[whichSixteenthAreWeIn];
		}

		Sample dry = buffer[bufferIteration];
		Sample wet = dry * Sample(cmult);
		buffer[bufferIteration] = aux::mixDryWet(dry, wet, drywet);
	}
}
#include "RhythmgateEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"
#include "BhMath.h"

RhythmgateEffect::RhythmgateEffect() : EffectBase(20, true)
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
		(params.getParameter(i)) = new FloatParameter(1.f, NormalizedRange(), name, "");
	}
	params.getParameter(16u) = new FloatParameter(0.1f, NormalizedRange(0.1f, 1700.f, 0.23f), "attack", "ms");
	params.getParameter(17u) = new FloatParameter(0.1f, NormalizedRange(0.1f, 1700.f, 0.23f), "release", "ms");
	params.getParameter(18u) = new FloatParameter(1.f, NormalizedRange(0.0625f, 2.f, 0.23f), "multiplier", "");
	params.getParameter(19u) = new FloatParameter(0.f, NormalizedRange(0.f, 6000.f, 0.37f), "offset", "ms");
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

	float mult = 0.0625;
	if (multiplier < 0.125f)
		mult = 0.0625;
	else if (multiplier < 0.25)
		mult = 0.125f;
	else if (multiplier < 0.5)
		mult = 0.25;
	else if (multiplier < 1.f)
		mult = 0.5;
	else if (multiplier < 1.5f)
		mult = 1.f;
	else if (multiplier >= 1.5)
		mult = 2.f;


	for (size_t bufferIteration = 0; bufferIteration < numberOfSamples; bufferIteration++)
	{
		float quarterNoteLength = (60.f /*seconds in a minute*/ * mult) / tempodata.bpm;
		float sixteenthNoteLength = quarterNoteLength / 4.f;
		float wholeBeatLength = sixteenthNoteLength * 16.f;
		float currentSecond = static_cast<float>(tempodata.position) / constants::sampleRate;

		if (offset != 0.f)
			if(offset > 0.f)
				currentSecond += offset / 1000.f;

		float currentPartialBeatInSecond = BhMath::fmod(currentSecond, wholeBeatLength);
		float currentPartialSixteenthInSeconds = BhMath::fmod(currentSecond, sixteenthNoteLength);
		float crelease = release;
		float cattack = attack;
		unsigned int whichSixteenthAreWeIn = static_cast<unsigned int>(currentPartialBeatInSecond / sixteenthNoteLength);
		whichSixteenthAreWeIn = whichSixteenthAreWeIn == 0u ? 15u : whichSixteenthAreWeIn - 1u;



		if (crelease > sixteenthNoteLength * 1000.f)
			crelease = sixteenthNoteLength * 1000.f - 2.f;
		if (cattack > sixteenthNoteLength * 1000.f - crelease)
			cattack = sixteenthNoteLength * 1000.f - crelease;
			
		unsigned int lastSixteenth = whichSixteenthAreWeIn == 0 ? 15 : whichSixteenthAreWeIn - 1u;
		unsigned int nextSixteenth = whichSixteenthAreWeIn == 15u ? 0u : whichSixteenthAreWeIn + 1u;


		if (params[lastSixteenth] > params[whichSixteenthAreWeIn]
			&& currentPartialSixteenthInSeconds * 1000.f < crelease)
		{
			float releaseFrac = (currentPartialSixteenthInSeconds * 1000.f / crelease);
			float cmult = params[lastSixteenth] + releaseFrac * (params[whichSixteenthAreWeIn] - params[lastSixteenth]);
			buffer[bufferIteration] *= Sample(cmult);
		}
		else if (params[nextSixteenth] > params[whichSixteenthAreWeIn]
			&& (sixteenthNoteLength - currentPartialSixteenthInSeconds) * 1000.f < cattack)
		{
			float attackFrac = 1.f - ((sixteenthNoteLength - currentPartialSixteenthInSeconds) * 1000.f) / cattack;
			float cmult = params[whichSixteenthAreWeIn] + attackFrac * (params[nextSixteenth] - params[whichSixteenthAreWeIn]);
			buffer[bufferIteration] *= Sample(cmult);
		}
		else
		{
			buffer[bufferIteration] *= Sample(params[whichSixteenthAreWeIn]);
		}
	}
}
#include "RhythmgateEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"
#include <cmath>

RhythmgateEffect::RhythmgateEffect() : EffectBase(20, true)
{
	// 16 levels,
	// attack release
	ParameterBundle* params = getPointerToParameterBundle();

	for (unsigned int i = 0u; i < 16u; i++)
	{
		(params->getParameter(i)) = new FloatParameter(1.f, NormalizedRange(), "gate" + std::to_string(i), "");
	}
	(params->getParameter(16u)) = new FloatParameter(0.1f, NormalizedRange(0.1f, 1700.f, 0.23f), "attack", "ms");
	(params->getParameter(17u)) = new FloatParameter(0.1f, NormalizedRange(0.1f, 1700.f, 0.23f), "release", "ms");
	(params->getParameter(18u)) = new FloatParameter(1.f, NormalizedRange(0.0625f, 2.f, 0.23f), "multiplier", "");
	(params->getParameter(19u)) = new FloatParameter(0.f, NormalizedRange(0.f, 6000.f, 0.37f), "offset", "ms");

}

void RhythmgateEffect::process(Sample* buffer, size_t numberOfSamples)
{

	InterpolatedValue<float>* params[16];
	for (unsigned int i = 0u; i < 16u; i++)
	{
		params[i] = &getInterpolatedParameter(i);
	}


	InterpolatedValue<float>& attack = getInterpolatedParameter(16u);
	InterpolatedValue<float>& release = getInterpolatedParameter(17u);

	InterpolatedValue<float>& multiplier = getInterpolatedParameter(18u);
	InterpolatedValue<float>& offset = getInterpolatedParameter(19u);

	float mult = 0.0625;
	if (multiplier.get() < 0.125f)
		mult = 0.0625;
	else if (multiplier.get() < 0.25)
		mult = 0.125f;
	else if (multiplier.get() < 0.5)
		mult = 0.25;
	else if (multiplier.get() < 1.f)
		mult = 0.5;
	else if (multiplier.get() < 1.5f)
		mult = 1.f;
	else if (multiplier.get() >= 1.5)
		mult = 2.f;


	// attack and release currently not used
	for (size_t bufferIteration = 0; bufferIteration < numberOfSamples; bufferIteration++)
	{
		float quarterNoteLength = (60.f /*seconds in a minute*/ * mult) / tempodata.bpm;
		float sixteenthNoteLength = quarterNoteLength / 4.f;
		float wholeBeatLength = sixteenthNoteLength * 16.f;
		float currentSecond = static_cast<float>(tempodata.position) / constants::sampleRate;

		if (offset.get() != 0.f)
			if(offset.get() > 0.f)
				currentSecond += offset.get() / 1000.f;

		float currentPartialBeatInSecond = fmod(currentSecond, wholeBeatLength);
		float currentPartialSixteenthInSeconds = fmod(currentSecond, sixteenthNoteLength);
		float crelease = release.get();
		float cattack = attack.get();
		unsigned int whichSixteenthAreWeIn = static_cast<unsigned int>(currentPartialBeatInSecond / sixteenthNoteLength);
		whichSixteenthAreWeIn = whichSixteenthAreWeIn == 0u ? 15u : whichSixteenthAreWeIn - 1u;



		if (crelease > sixteenthNoteLength * 1000.f)
			crelease = sixteenthNoteLength * 1000.f - 2.f;
		if (cattack > sixteenthNoteLength * 1000.f - crelease)
			cattack = sixteenthNoteLength * 1000.f - crelease;
			
		unsigned int lastSixteenth = whichSixteenthAreWeIn == 0 ? 15 : whichSixteenthAreWeIn - 1u;
		unsigned int nextSixteenth = whichSixteenthAreWeIn == 15u ? 0u : whichSixteenthAreWeIn + 1u;


		if (params[lastSixteenth]->get() > params[whichSixteenthAreWeIn]->get()
			&& currentPartialSixteenthInSeconds * 1000.f < crelease)
		{
			float releaseFrac = (currentPartialSixteenthInSeconds * 1000.f / crelease);
			float cmult = params[lastSixteenth]->get() + releaseFrac * (params[whichSixteenthAreWeIn]->get() - params[lastSixteenth]->get());
			buffer[bufferIteration] *= Sample(cmult);
		}
		else if (params[nextSixteenth]->get() > params[whichSixteenthAreWeIn]->get()
			&& (sixteenthNoteLength - currentPartialSixteenthInSeconds) * 1000.f < cattack)
		{
			float attackFrac = 1.f - ((sixteenthNoteLength - currentPartialSixteenthInSeconds) * 1000.f) / cattack;
			float cmult = params[whichSixteenthAreWeIn]->get() + attackFrac * (params[nextSixteenth]->get() - params[whichSixteenthAreWeIn]->get());
			buffer[bufferIteration] *= Sample(cmult);
		}
		else
		{
			buffer[bufferIteration] *= Sample(params[whichSixteenthAreWeIn]->get());
		}

		this->nextSample();
		tempodata.position++;
	}
}
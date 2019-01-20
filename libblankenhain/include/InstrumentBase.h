#pragma once

#include "EffectBase.h"
#include "Constants.h"
#include "AuxFunc.h"
#include "MidiEvent.h"

class VoiceState;

class InstrumentBase : public EffectBase
{
public:
	InstrumentBase(unsigned int numberOfParameters, unsigned int numberOfVoices, bool usesTempoData = false);

	void handleNoteEvent(bool isNoteOn, unsigned int key, unsigned int velocity);

	virtual void processVoice(VoiceState& voice, unsigned int timeInSamples, Sample* buffer, size_t numberOfSamples) = 0;

#ifndef _BLANKENHAIN_RUNTIME_MODE
	/**
	 * Resets voices, calles when plugin is turned off
	 * usefull since sometimes when working in a DAW, midiOff
	 * messages can get lost and then the synth goes full retard.
	 * Turning it on and off should now fix this :D
	 */
	void resetVoices();
#endif
protected:
	const unsigned int numberOfVoices;
	virtual void process(Sample* buffer, size_t numberOfSamples, size_t currentTime) final;
private:
	Sample voiceBuffer[constants::blockSize];
	VoiceState* voices;
};

template <typename T>
float performAHDSR(
	T* buffer, VoiceState& voice, unsigned int timeInSamples, unsigned int sampleIndex,
	float attackInMs, float releaseInMs, float holdInMs = 1.f, float decayInMs = 1.f, float sustainInMs = 1.f,
	bool useSustainEvenIfNoteStillOn = false, float normalizedSustainLevel = 1.f, float normalizedHoldLevel = 1.f,
	float rampStartingFromThisMultiplier = 0.f
)
{
	// Some more general behavior: If holdLevel is 0, it is assumed to user wants a simple attack-sustain-release curve without hold.
	// Thus, holdTime is assumed to be 0 samples so that attack is directly followed by sustain. Normalized hold level is for this code
	// then equal to the normalized sustain level
	if (normalizedHoldLevel < 0.0001f)
	{
		normalizedHoldLevel = normalizedSustainLevel;
		holdInMs = 0.f;
		decayInMs = 0.f;
	}

	const float attackInSamples = aux::millisecToSamples(attackInMs);
	const float holdInSamples = aux::millisecToSamples(holdInMs);
	const float decayInSamples = aux::millisecToSamples(decayInMs);
	const float sustainInSamples = aux::millisecToSamples(sustainInMs);
	const float releaseInSamples = aux::millisecToSamples(releaseInMs);
	const unsigned int deltaTinSamples = (timeInSamples + sampleIndex) - voice.onTime;

	float multiplierCurrentlyUsed = 0.f;

	if (voice.isOn)
	{
		if (attackInSamples != 0.f)
		{
			if (deltaTinSamples <= attackInSamples)
			{
				multiplierCurrentlyUsed = (deltaTinSamples / attackInSamples) * normalizedHoldLevel + rampStartingFromThisMultiplier * (1.f - (deltaTinSamples / attackInSamples));
			}
			else if (deltaTinSamples < attackInSamples + holdInSamples)
			{
				multiplierCurrentlyUsed = normalizedHoldLevel;
			}
			else if (deltaTinSamples < attackInSamples + holdInSamples + decayInSamples)
			{
				if(decayInSamples != 0.f)
					multiplierCurrentlyUsed = (normalizedSustainLevel - normalizedHoldLevel) / decayInSamples * (deltaTinSamples - attackInSamples - holdInSamples) + normalizedHoldLevel;
				else
					multiplierCurrentlyUsed = normalizedHoldLevel;
			}
			else
			{
				multiplierCurrentlyUsed = normalizedSustainLevel;
				if (useSustainEvenIfNoteStillOn && deltaTinSamples > attackInSamples + holdInSamples + decayInSamples + sustainInSamples)
					voice.off(timeInSamples + sampleIndex);
			}
		}
	}
	else
	{
		unsigned int deltaToff = (timeInSamples + sampleIndex) - voice.offTime;
		deltaToff = deltaToff == 0 ? 1 : deltaToff;
		if (deltaToff < static_cast<unsigned int>(releaseInSamples))
		{
			float lastAmplitudeRelative = 1.f;

			if (deltaTinSamples <= attackInSamples)
				lastAmplitudeRelative = deltaTinSamples / attackInSamples * normalizedHoldLevel;
			else if (deltaTinSamples <= attackInSamples + holdInSamples)
				lastAmplitudeRelative = normalizedHoldLevel;
			else if (deltaTinSamples < attackInSamples + decayInSamples + holdInSamples
				&& decayInSamples != 0.f)
				lastAmplitudeRelative = (normalizedSustainLevel - normalizedHoldLevel) / decayInSamples * (deltaTinSamples - attackInSamples - holdInSamples) + normalizedHoldLevel;
			else
				lastAmplitudeRelative = normalizedSustainLevel;
			const float releaseFactor = (1 - (deltaToff / releaseInSamples)) * lastAmplitudeRelative;
			multiplierCurrentlyUsed = releaseFactor;
		}
		else
		{
			// switch off note immediately
			multiplierCurrentlyUsed = 0.f;
			voice.isSilent = true;
		}
	}
	buffer[sampleIndex] *= T(multiplierCurrentlyUsed);
	return multiplierCurrentlyUsed;
}
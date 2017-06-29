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
void performAHDSR(
	T* buffer, VoiceState& voice, unsigned int timeInSamples, unsigned int sampleIndex,
	float attackInMs, float releaseInMs, float holdInMs = 1.f, float decayInMs = 1.f, float sustainInMs = 1.f,
	bool useSustainEvenIfNoteStillOn = false, float normalizedSustainLevel = 1.f, float normalizedHoldLevel = 1.f
)
{
	float attackInSamples = aux::millisecToSamples(attackInMs);
	float holdInSamples = aux::millisecToSamples(holdInMs);
	float decayInSamples = aux::millisecToSamples(decayInMs);
	float sustainInSamples = aux::millisecToSamples(sustainInMs);
	float releaseInSamples = aux::millisecToSamples(releaseInMs);
	unsigned int deltaTinSamples = (timeInSamples + sampleIndex) - voice.onTime;

	if (voice.isOn)
	{
		if (attackInSamples != 0.f)
		{
			if (deltaTinSamples <= attackInSamples)
			{
				buffer[sampleIndex] *= T((deltaTinSamples / attackInSamples) * normalizedHoldLevel);
			}
			else if (deltaTinSamples < attackInSamples + holdInSamples)
			{
				buffer[sampleIndex] *= T(normalizedHoldLevel);
			}
			else if (deltaTinSamples < attackInSamples + holdInSamples + decayInSamples)
			{
				if(decayInSamples != 0.f)
					buffer[sampleIndex] *= T((normalizedSustainLevel - normalizedHoldLevel) / decayInSamples * (deltaTinSamples - attackInSamples - holdInSamples) + normalizedHoldLevel);
				else
					buffer[sampleIndex] *= T(normalizedHoldLevel);
			}
			else
			{
				buffer[sampleIndex] *= T(normalizedSustainLevel);
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
			float releaseFactor = (1 - (deltaToff / releaseInSamples)) * lastAmplitudeRelative;
			buffer[sampleIndex] *= T(releaseFactor);
		}
		else
		{
			// switch off note immediately
			buffer[sampleIndex] = T(0.f);
			voice.isSilent = true;
		}
	}
}
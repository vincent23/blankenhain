#include "InstrumentBase.h"
#include "VoiceState.h"
#include "BhMath.h"

InstrumentBase::InstrumentBase(unsigned int numberOfParameters, unsigned int numberOfVoices_, bool usesTempoData)
	: EffectBase(numberOfParameters, usesTempoData)
	, numberOfVoices(numberOfVoices_)
	, voices(new VoiceState[numberOfVoices_])
{ }

void InstrumentBase::handleNoteEvent(bool isNoteOn, unsigned int key, unsigned int velocity)
{
	if (isNoteOn) {
		// note on: select a voice for the new note and set it to on.
		// voice selection is performed in order of the following list:
		// 1. silent voice
		// 2. earliest off voice same key (0b11)
		// 3. earliest off voice (0b10)
		// 4. earliest on voice same key (0b01)
		// 5. earliest on voice (0b00)
		// we compute a priority value for each voice(given in parenthesis) to compare them
		VoiceState* selectedVoice = nullptr;
		unsigned int earliestTime = 0;
		unsigned int priorityValue = 0;
		for (unsigned int voiceIndex = 0; voiceIndex < numberOfVoices; voiceIndex++) {
			VoiceState& voice = voices[voiceIndex];
			if (voice.isSilent) {
				// a silent voice can be used in any case
				selectedVoice = &voice;
				break;
			}
			else {
				unsigned int newPriorityValue = ((voice.isOn ? 0 : 1) << 1) | (voice.key == key ? 1 : 0);
				if (selectedVoice == nullptr
					|| priorityValue < newPriorityValue
					|| (priorityValue == newPriorityValue && voice.time() < selectedVoice->time()))
				{
					// either:
					// - we don't have any voice
					// - we found a voice with higher priority
					// - the new voice has the same priority and is earlier
					selectedVoice = &voice;
					priorityValue = newPriorityValue;
				}
			}
		}
		if (selectedVoice != nullptr) {
			selectedVoice->on(this->getCurrentTime(), key, velocity);
		}
	}
	else {
		// note off: find the earliest note that is on with the same key and set the voice to off
		unsigned int earliestTime = 0xffffffff;
		VoiceState* earliestNote = nullptr;
		for (unsigned int voiceIndex = 0; voiceIndex < numberOfVoices; voiceIndex++) {
			VoiceState& voice = voices[voiceIndex];
			if (!voice.isSilent && voice.key == key && voice.isOn && voice.onTime <= earliestTime) {
				earliestNote = &voice;
			}
		}
		if (earliestNote != nullptr) {
			earliestNote->off(this->getCurrentTime());
		}
	}
}

#ifndef _BLANKENHAIN_RUNTIME_MODE
void InstrumentBase::resetVoices()
{
	for (unsigned int voiceIndex = 0; voiceIndex < numberOfVoices; voiceIndex++) 
	{
		voices[voiceIndex].isOn = false;
	}
}
#endif

void InstrumentBase::process(Sample* buffer, size_t numberOfSamples, size_t currentTime)
{
	for (unsigned int sampleIndex = 0; sampleIndex < numberOfSamples; sampleIndex++) {
		buffer[sampleIndex] = Sample(0);
	}

	for (unsigned int voiceIndex = 0; voiceIndex < numberOfVoices; voiceIndex++) {
		VoiceState& voice = voices[voiceIndex];
		if (voice.isSilent) {
			continue;
		}
		processVoice(voice, currentTime, voiceBuffer, numberOfSamples);
		for (unsigned int sampleIndex = 0; sampleIndex < numberOfSamples; sampleIndex++) {
			Sample& currentSample = voiceBuffer[sampleIndex];
			// via "normalizeed value" skewing with factor 1.
			// This can be adjusted to give a more natural behaviour maybe?
			// TODO 
			//float skew = 1.f;
			//float velocityVolumeScaling = BhMath::pow(static_cast<float>(voice.velocity) / 127.f, 1.f / skew);
			// BHMATH pow was incredibly slow
			float velocityVolumeScaling = (static_cast<float>(voice.velocity) / 127.f);
			currentSample *= Sample(velocityVolumeScaling);
			buffer[sampleIndex] += currentSample;
		}
	}
}
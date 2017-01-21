#include "glidePolyblepInstrument.h"
#include "InterpolatedValue.h"
#include "ParameterBundle.h"
#include "FloatParameter.h"
#include "VoiceState.h"

#include <cmath>

glidePolyblepInstrument::glidePolyblepInstrument()
	: InstrumentBase(13, 1)
{
	ParameterBundle* params = getPointerToParameterBundle();

	params->getParameter(0) = new FloatParameter(50.f, NormalizedRange(0.1f, 1700.f, 0.3f), "attack", "ms");
	params->getParameter(1) = new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "hold", "ms");
	params->getParameter(2) = new FloatParameter(1.f, NormalizedRange(), "holdlevel", "ratio");
	params->getParameter(3) = new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "decay", "ms");
	params->getParameter(4) = new FloatParameter(0.f, NormalizedRange(), "sustainBool", "bool");
	params->getParameter(5) = new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "sustain", "ms");
	params->getParameter(6) = new FloatParameter(1.0f, NormalizedRange(), "sustainLevel", "ratio");
	params->getParameter(7) = new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "release", "ms");
	params->getParameter(8) = new FloatParameter(0.f, NormalizedRange(0.f, 3.9f), "osc", "");
	params->getParameter(9) = new FloatParameter(0.f, NormalizedRange(0.f, 0.3f), "glide", "");

	params->getParameter(10) = new FloatParameter(0.f, NormalizedRange(0.f, 1.f), "lfoAmount", "");
	params->getParameter(11) = new FloatParameter(0.f, NormalizedRange(0.f, 2.9f), "lfoSpeed", "");
	params->getParameter(12) = new FloatParameter(0.f, NormalizedRange(-5.f, 5.0f), "detune", "");
	// Lfo einfach in getModulation schreiben. Hier sin() machen und auf detune drauf schreiben. weiß noch nicht ganz was dann passiert, aber wird schon passen
	// Der lfo einach von fmSynth kopieren.

	freqPrev = 0.f;
	timeNoteOff = 0u;
	notePrev = 0u;
}

glidePolyblepInstrument::~glidePolyblepInstrument()
{
}

void glidePolyblepInstrument::processVoice(VoiceState& voice, unsigned int timeInSamples, Sample* buffer, unsigned int numberOfSamples)
{

	float attack = getInterpolatedParameter(0).get();
	float hold = getInterpolatedParameter(1).get();
	float holdLevel = getInterpolatedParameter(2).get();
	float decay = getInterpolatedParameter(3).get();
	bool sustainOn = getInterpolatedParameter(4).get() > 0.5 ? true : false;
	float sustainLevel = getInterpolatedParameter(6).get();
	float sustain = getInterpolatedParameter(5).get();
	float release = getInterpolatedParameter(7).get();
	unsigned int oscMode = static_cast<unsigned int>(getInterpolatedParameter(8).get());
	float portamento = getInterpolatedParameter(9).get();
	float detune = getInterpolatedParameter(12).get();

	// oscMode 1: polyBLEP Sawtooth
	// oscMode 2: polyBLEP Square (broken)
	// oscMode 3: polyBLEP Triangle (broken)

	this->osc.setMode(NaiveOscillator::NaiveOscillatorMode(oscMode));

	// If a new note is played, take this as the start time for glide
	// TODO we can improve this but as this synth only has one voice
	// Sometimes we will not get NoteOff events, so this crude
	// check will have to do for now.
    if (notePrev != voice.key)
	{
		notePrev = voice.key;
		timeNoteOff = voice.onTime;
	}

	float voiceFreq = aux::noteToFrequency(voice.key);
	if (detune != 0.f)
		voiceFreq = aux::calculateDetune(voiceFreq, detune, 5u);


	for (unsigned int sampleIndex = 0; sampleIndex < numberOfSamples; sampleIndex++)
	{
		//unsigned int deltaT = (timeInSamples + sampleIndex) - voice.onTime;
		unsigned int timeSinceNoteOff = (timeInSamples + sampleIndex) - timeNoteOff;
		float currentFreq;

		// If enough glide time has past, set previousFreq to currentFreq and thereby stop gliding
		if (freqPrev != voiceFreq
			&& exp(-1.f * timeSinceNoteOff / (constants::sampleRate * portamento)) < 0.0001)
		{
			notePrev = voice.key;
			freqPrev = voiceFreq;
		}

		// If subsequent note has same freq or portamento is off
		// There will be no glide
		if ((portamento == 0.f
			|| freqPrev == voiceFreq))
			currentFreq = voiceFreq;
		else
			// otherwise we glide
			// formula via http://stanford.edu/~yanm2/files/mus420b.pdf
		    currentFreq = voiceFreq * (1.f - exp(-1.f * timeSinceNoteOff / (constants::sampleRate * portamento) )) +
			    freqPrev * exp(-1.f * timeSinceNoteOff / (constants::sampleRate * portamento) );

		// evalutate Frequency as usual
		this->osc.setFrequency(currentFreq);
		buffer[sampleIndex] = Sample(this->osc.getNextSample());
		performAHDSR<Sample>(buffer, voice, timeInSamples, sampleIndex, attack, release, hold, decay, sustain, sustainOn, sustainLevel, holdLevel);
	}
}
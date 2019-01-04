#include "glidePolyblepInstrument.h"
#include "InterpolatedValue.h"
#include "ParameterBundle.h"
#include "FloatParameter.h"
#include "VoiceState.h"

glidePolyblepInstrument::glidePolyblepInstrument()
	: InstrumentBase(18, 1, true), noise(), currentOsc(&noise), lastUsedAHDSRMultiplier(0.f)
{
	ParameterBundle& params = getParameterBundle();

	params.initParameter(0, new FloatParameter(50.f, NormalizedRange(0.1f, 1700.f, 0.3f), "attack", "ms"));
	params.initParameter(1, new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "hold", "ms"));
	params.initParameter(2, new FloatParameter(1.f, NormalizedRange(), "holdlevel", "ratio"));
	params.initParameter(3, new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "decay", "ms"));
	params.initParameter(4, new BoolParameter(false, "sustainBool"));
	params.initParameter(5, new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "sustain", "ms"));
	params.initParameter(6, new FloatParameter(1.0f, NormalizedRange(), "sustainLevel", "ratio"));
	params.initParameter(7, new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "release", "ms"));
	BhString names[5] = { "sine", "saw", "square", "triangle", "noise" };
	params.initParameter(8, new OptionParameter(5u, names, "osc", ""));
	params.initParameter(9, new FloatParameter(0.f, NormalizedRange(0.f, 0.3f), "glide", ""));
	params.initParameter(10, new FloatParameter(0.f, NormalizedRange(-1.f, 1.f), "lfoAmount", ""));
	params.initParameter(11, new FloatParameter(0.0055f, NormalizedRange(0.005f, 20.f, 0.325f), "lfoSpeed", ""));
	params.initParameter(12, new FloatParameter(0.f, NormalizedRange(-5.f, 5.0f), "detune", ""));
	float multiplierValues[7] = { 0.0625, 0.125, 0.25, 0.5, 1., 2., 4. };
	params.initParameter(13, new DiscreteParameter(7u, "lfoBeatMultiplier", "", multiplierValues));
	params.initParameter(14, new OptionParameter(4u, names, "lfoWaveform", ""));
	params.initParameter(15, new BoolParameter(false, "lfoTemposync"));

	params.initParameter(16, new FloatParameter(0.f, NormalizedRange(0.f, 2.f * 3.14159265359f), "lfoPhase", ""));
	params.initParameter(17, new FloatParameter(0.f, NormalizedRange(-1.f, 1.f), "lfoBaseline", ""));

	// Lfo einfach in getModulation schreiben. Hier sin() machen und auf detune drauf schreiben. wei� noch nicht ganz was dann passiert, aber wird schon passen
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
	float attack = interpolatedParameters.get(0);
	float hold = interpolatedParameters.get(1);
	float holdLevel = interpolatedParameters.get(2);
	float decay = interpolatedParameters.get(3);
	bool sustainOn = interpolatedParameters.get(4) > 0.5 ? true : false;
	float sustainLevel = interpolatedParameters.get(6);
	float sustain = interpolatedParameters.get(5);
	float release = interpolatedParameters.get(7);
	unsigned int oscMode = static_cast<unsigned int>(interpolatedParameters.get(8));
	float portamento = interpolatedParameters.get(9);
	float detune = interpolatedParameters.get(12);
	// oscMode 0: polyBLEP Sine
	// oscMode 1: polyBLEP Sawtooth
	// oscMode 2: polyBLEP Square
	// oscMode 3: polyBLEP Triangle
	// oscMode 4: noise

	if (oscMode >= 4u)
	{
		currentOsc = &noise;
	}
	else
	{
		currentOsc = &osc;
		this->osc.setMode(NaiveOscillator::NaiveOscillatorMode(static_cast<unsigned int>(oscMode)));
	}

	// If a new note is played, take this as the start time for glide
	// TODO we can improve this but as this synth only has one voice
	// Sometimes we will not get NoteOff events, so this crude
	// check will have to do for now.
    if (notePrev != voice.key)
	{
		notePrev = voice.key;
		timeNoteOff = voice.onTime;
	}

	float voiceFreq = aux::noteToFrequency(static_cast<float>(voice.key));
	if (detune != 0.f)
		voiceFreq = aux::calculateDetune(voiceFreq, detune, 5u);


	for (unsigned int sampleIndex = 0; sampleIndex < numberOfSamples; sampleIndex++)
	{
		//unsigned int deltaT = (timeInSamples + sampleIndex) - voice.onTime;
		unsigned int timeSinceNoteOff = (timeInSamples + sampleIndex) - timeNoteOff;
		float currentFreq;

		// If enough glide time has past, set previousFreq to currentFreq and thereby stop gliding
		if (portamento != 0.f
			&& freqPrev != voiceFreq
			&& BhMath::exp(-1.f * timeSinceNoteOff / (constants::sampleRate * portamento)) < 0.0001)
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
		    currentFreq = voiceFreq * (1.f - BhMath::exp(-1.f * timeSinceNoteOff / (constants::sampleRate * portamento) )) +
			    freqPrev * BhMath::exp(-1.f * timeSinceNoteOff / (constants::sampleRate * portamento) );

		// evalutate Frequency as usual
		currentOsc->setFrequency(currentFreq);
		buffer[sampleIndex] = Sample(currentOsc->getNextSample());
		if (timeSinceNoteOff <= aux::millisecToSamples(attack))
			performAHDSR<Sample>(buffer, voice, timeInSamples, sampleIndex, attack, release, hold, decay, sustain, sustainOn, sustainLevel, holdLevel,lastUsedAHDSRMultiplier);
		else
			lastUsedAHDSRMultiplier = performAHDSR<Sample>(buffer, voice, timeInSamples, sampleIndex, attack, release, hold, decay, sustain, sustainOn, sustainLevel, holdLevel);
	}
}

void glidePolyblepInstrument::getModulation(float* modulationValues, size_t sampleOffset)
{
	float lfoAmount = interpolatedParameters.get(10);

	// Perform LFO on detune
	if (lfoAmount != 0.f)
	{
		float lfoBaseline = interpolatedParameters.get(17);

		float lfoWaveform = interpolatedParameters.get(14);
		bool lfoTempoSync = interpolatedParameters.get(15) == 1.f;
		this->lfo.setMode(NaiveOscillator::NaiveOscillatorMode(static_cast<unsigned int>(lfoWaveform)));
		float lfoPhase = interpolatedParameters.get(16);
		if (!lfoTempoSync)
		{
			float lfoSpeed = interpolatedParameters.get(11);
			this->lfo.setFrequency(lfoSpeed);
		}
		else
		{
			float lfoMult = interpolatedParameters.get(13);
			if (lfoMult < 0.125f)
				lfoMult = 0.0625;
			else if (lfoMult < 0.25)
				lfoMult = 0.125f;
			else if (lfoMult < 0.5)
				lfoMult = 0.25;
			else if (lfoMult < 1.f)
				lfoMult = 0.5;
			else if (lfoMult < 1.9f)
				lfoMult = 1.f;
			else if (lfoMult < 3.5f)
				lfoMult = 2.f;
			else
				lfoMult = 4.f;

			float quarterNoteLength = (60.f /*seconds in a minute*/ * lfoMult) / tempodata.bpm;
			float sixteenthNoteLength = quarterNoteLength / 4.f;
			float wholeBeatLength = sixteenthNoteLength * 16.f;

			this->lfo.setFrequency(2.f / wholeBeatLength);
		}
		
		this->lfo.setParams(lfoBaseline, OscillatorPhase(lfoPhase), lfoAmount);
		for (unsigned int i = 0u; i < sampleOffset; i++)
		{
			if (this->effectUsesTempoData())
				modulationValues[12] = this->lfo.getSample(sampleOffset + this->tempodata.position);
			else
				modulationValues[12] = this->lfo.getNextSample();
		}
	}

};
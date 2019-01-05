#include "FmInstrument.h"
#include "InterpolatedValue.h"
#include "ParameterBundle.h"
#include "FloatParameter.h"
#include "VoiceState.h"

//Todo selfmod!

FmInstrument::FmInstrument()
	: InstrumentBase(8+ _LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH*13-13+4, 1, true), currentSound(nullptr), lastCarrierValue(1.f), lastUsedAHDSRMultiplier(0.f)
{
	ParameterBundle& params = getParameterBundle();

	// envelope global
	params.initParameter(0, new FloatParameter(50.f, NormalizedRange(1.f, 1700.f, 0.3f), "attack", "ms"));
	params.initParameter(1, new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "hold", "ms"));
	params.initParameter(2, new FloatParameter(1.f, NormalizedRange(), "holdlevel", "ratio"));
	params.initParameter(3, new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "decay", "ms"));
	params.initParameter(4, new BoolParameter(false, "sustainBool"));
	params.initParameter(5, new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "sustain", "ms"));
	params.initParameter(6, new FloatParameter(1.0f, NormalizedRange(), "sustainLevel", "ratio"));
	params.initParameter(7, new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "release", "ms"));


	// Modulation Oscillators
	for (unsigned int i = 0u; i < _LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - 1u; i++)
	{
		unsigned int currentIter = 8u + i * 13;
		params.initParameter(currentIter + 0, new FloatParameter(440.f, NormalizedRange(33.f, 16000.f, 0.25), "freq", "hz"));
		BhString names[2] = { "PM", "AM" };
		params.initParameter(currentIter + 1, new OptionParameter(2u, names, "modType", ""));
		params.initParameter(currentIter + 2, new FloatParameter(0.f, NormalizedRange(), "modAmount", "amount"));

		float* numbersOfOsc = new float[_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - i];
		for (unsigned int j = i; j < _LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - 1; j++)
		{
			unsigned int accessArray = j - i;
			numbersOfOsc[accessArray] = static_cast<float>(j + 1);
		}

		params.initParameter(currentIter + 3, new DiscreteParameter(_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - i - 1, "target", "", numbersOfOsc, _LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - i - 2));
		delete[] numbersOfOsc;

		params.initParameter(currentIter + 4, new FloatParameter(0.f, NormalizedRange(), "selfmodAmount", "amount"));
		BhString waveform[4] = { "Sine", "Saw", "Square", "Triangle" };
		params.initParameter(currentIter + 5, new OptionParameter(2u, names, "SelfmodType", ""));
		params.initParameter(currentIter + 6, new OptionParameter(4u, waveform, "waveform", ""));
		params.initParameter(currentIter + 7, new BoolParameter(false, "isOn"));
		params.initParameter(currentIter + 8, new BoolParameter(false, "isLFO"));
		params.initParameter(currentIter + 9, new BoolParameter(false, "tempoSyncOn"));
		float multiplierValues[7] = { 0.0625, 0.125, 0.25, 0.5, 1., 2., 4. };
		params.initParameter(currentIter + 10, new DiscreteParameter( 7u, "multiplier", "", multiplierValues, 4u));
		params.initParameter(currentIter + 11, new BoolParameter(false, "TrackKey"));

		float* trackKeyShiftOptions = new float[9u];
		for (unsigned int j = 0u; j < 9u; j++)
		{
			trackKeyShiftOptions[j] = static_cast<float>(-4 + static_cast<int>(j));
		}

		params.initParameter(currentIter + 12, new DiscreteParameter(9u, "TrackKeyShift","", trackKeyShiftOptions, 4u));
		delete[] trackKeyShiftOptions;

	}

	BhString names[2] = { "PM", "AM" };
	BhString waveform[4] = { "Sine", "Saw", "Square", "Triangle" };
	//Carrier
	params.initParameter(60, new FloatParameter(0.f, NormalizedRange(), "selfmodAmount", "amount"));
	params.initParameter(61, new OptionParameter(2u, names, "SelfmodType", ""));
	params.initParameter(62, new OptionParameter(4u, waveform, "waveform", ""));
	params.initParameter(63, new FloatParameter(0.f, NormalizedRange(0.f, 0.3f), "glide", ""));

	for (unsigned int i = 0u; i < _LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH; i++)
	{
		osc[i].setMode(NaiveOscillator::NaiveOscillatorMode::OSCILLATOR_MODE_SINE);
		lastOscValues[i] = 1.f;
	}
	currentSound = &osc[_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH -1];
}

void FmInstrument::processVoice(VoiceState& voice, unsigned int timeInSamples, Sample* buffer, unsigned int numberOfSamples)
{
	const float attack = interpolatedParameters.get(0);
	const float hold = interpolatedParameters.get(1);
	const float holdLevel = interpolatedParameters.get(2);
	const float decay = interpolatedParameters.get(3);
	const bool sustainOn = interpolatedParameters.get(4) > 0.5 ? true : false;
	const float sustainLevel = interpolatedParameters.get(6);
	const float sustain = interpolatedParameters.get(5);
	const float release = interpolatedParameters.get(7);

	// Portamento stuff
	const float portamento = interpolatedParameters.get(63);
	// If a new note is played, take this as the start time for glide
	// TODO we can improve this but as this synth only has one voice
	// Sometimes we will not get NoteOff events, so this crude
	// check will have to do for now.
	if (notePrev != voice.key)
	{
		notePrev = voice.key;
		timeNoteOff = voice.onTime;
	}

	const float voiceFreq = aux::noteToFrequency(static_cast<float>(voice.key));

	// reset modulation matrix
	for (unsigned int i = 0u; i < _LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH; i++)
		this->mod[i] = FmModulation();

	// Modulation Oscs are now evaluated
	for (unsigned int sampleIndex = 0; sampleIndex < numberOfSamples; sampleIndex++)
	{
		//unsigned int deltaT = (timeInSamples + sampleIndex) - voice.onTime;
		const unsigned int timeSinceNoteOff = (timeInSamples + sampleIndex) - timeNoteOff;
		float currentFreq;

		// If enough glide time has past, set previousFreq to currentFreq and thereby stop gliding
		if (freqPrev != voiceFreq
			&& portamento != 0.f
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
			currentFreq = voiceFreq * (1.f - BhMath::exp(-1.f * timeSinceNoteOff / (constants::sampleRate * portamento))) + \
			freqPrev * BhMath::exp(-1.f * timeSinceNoteOff / (constants::sampleRate * portamento));


		for (unsigned int i = 0u; i < _LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - 1u; i++)
		{
			const float amount = interpolatedParameters.get(8 + i * 13 + 2);
			const bool isOn = interpolatedParameters.get(8 + i * 13 + 7) == 1.f;
			if (isOn)
			{
				float freq = interpolatedParameters.get(8 + i * 13 + 0);
				const float modType = interpolatedParameters.get(8 + i * 13 + 1);
				const float target = interpolatedParameters.get(8 + i * 13 + 3);
				const float selfModAmount = interpolatedParameters.get(8 + i * 13 + 4) * 0.0001f;
				const bool selfModOn = selfModAmount > 0.0000000001f;
				const float selfModtype = interpolatedParameters.get(8 + i * 13 + 5);
				const float waveFormType = interpolatedParameters.get(8 + i * 13 + 6);
				const bool isLFO = interpolatedParameters.get(8 + i * 13 + 8) == 1.f;
				const bool tempoSync = interpolatedParameters.get(8 + i * 13 + 9) == 1.f;
				float tempoSyncVal = interpolatedParameters.get(8 + i * 13 + 10);
				const bool keyFreqTrack = interpolatedParameters.get(8 + i * 13 + 11) == 1.f;
				const float keyFreqOctave = interpolatedParameters.get(8 + i * 13 + 12);


				float freqMultiplierFM = 1.f;
				float freqAdditionPM = 0.f;

				// Now determine the frequency
				if (keyFreqTrack) 
					freq = aux::noteToFrequency(static_cast<float>(voice.key) + keyFreqOctave * 12.f);
				else
				{
					if (!tempoSync && isLFO)
						freq /= 1000.f;
					else if (tempoSync && isLFO)
					{
						if (tempoSyncVal < 0.125f)
							tempoSyncVal = 0.0625;
						else if (tempoSyncVal < 0.25)
							tempoSyncVal = 0.125f;
						else if (tempoSyncVal < 0.5)
							tempoSyncVal = 0.25;
						else if (tempoSyncVal < 1.f)
							tempoSyncVal = 0.5;
						else if (tempoSyncVal < 1.5f)
							tempoSyncVal = 1.f;
						else if (tempoSyncVal >= 1.5)
							tempoSyncVal = 2.f;

						const float quarterNoteLength = (60.f /*seconds in a minute*/ * tempoSyncVal) / tempodata.bpm;
						const float sixteenthNoteLength = quarterNoteLength / 4.f;
						const float wholeBeatLength = sixteenthNoteLength * 16.f;
						freq = 1.f / wholeBeatLength;
					}
					else if (tempoSync && !isLFO)
					{
						if (tempoSyncVal < 0.125f)
							tempoSyncVal = 0.0625;
						else if (tempoSyncVal < 0.25)
							tempoSyncVal = 0.125f;
						else if (tempoSyncVal < 0.5)
							tempoSyncVal = 0.25;
						else if (tempoSyncVal < 1.f)
							tempoSyncVal = 0.5;
						else if (tempoSyncVal < 1.5f)
							tempoSyncVal = 1.f;
						else if (tempoSyncVal >= 1.5)
							tempoSyncVal = 2.f;

						freq = aux::noteToFrequency(static_cast<float>(voice.key) + 12.f * tempoSyncVal);
					}
				}

				osc[i].setMode(NaiveOscillator::NaiveOscillatorMode(static_cast<int>(waveFormType)));

				if (mod[i].pm || (selfModOn && selfModtype == 0.f))
				{
					float selfmod = 0.f;
					if (selfModOn && selfModtype == 0.f)
					{
						selfmod = lastOscValues[i];
					}

					const float cPmVal = mod[i].pm ? mod[i].pmVal : 0.f;

					freqAdditionPM = (mod[i].pmVal * mod[i].pmAmount) + selfmod  * selfModAmount;
				}

				osc[i].setFrequency(freq + freqMultiplierFM);

				OscillatorPhase currentPhase;
				currentPhase.set(freqAdditionPM);

				float val = osc[i].getNextSample(currentPhase);

				if (mod[i].am || (selfModOn && selfModtype == 1.f))
				{
					float selfmod = 0.f;
					if (selfModOn && selfModtype == 1.f)
					{
						selfmod = lastOscValues[i];
					}

					float cAmAmount = 0.f;
					if (mod[i].am)
						cAmAmount = mod[i].amAmount;
					else
						cAmAmount = mod[i].amVal = 1.f;

					val *= cAmAmount * mod[i].amVal + selfmod * selfModAmount;
				}

				lastOscValues[i] = val;

				if (modType == 0.f)
				{
					mod[unsigned int(target)].pmVal = val;
					mod[unsigned int(target)].pmAmount = amount;
					mod[unsigned int(target)].pm = true;
				}
				else
				{
					mod[unsigned int(target)].amVal = val;
					mod[unsigned int(target)].amAmount = amount;
					mod[unsigned int(target)].am = true;
				}	
			}
		}

		// Carrier now

		const float freq = currentFreq;
		const float selfModAmount = interpolatedParameters.get(60);
		const bool selfModOn = selfModAmount > 1e-15;
		const float selfModtype = interpolatedParameters.get(61);
		const float waveFormType = interpolatedParameters.get(62);

		float freqMultiplierFM = 1.f;
		float freqAdditionPM = 0.f;

		osc[(_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - 1u)].setMode(NaiveOscillator::NaiveOscillatorMode(static_cast<int>(waveFormType)));


		if (mod[(_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - 1u)].pm || (selfModOn && selfModtype < 0.5f))
		{
			float selfmod = 0.f;
			if (selfModOn && selfModtype < 0.5f)
			{
				selfmod = lastCarrierValue;
			}

			float cPmVal = 0.f;
			if (mod[(_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - 1u)].pm)
				cPmVal = mod[(_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - 1u)].pmVal;

			freqAdditionPM = (cPmVal * mod[(_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - 1u)].pmAmount) + (selfModAmount * selfmod);
		}

		///////////////////////
		osc[(_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - 1u)].setFrequency(freq + freqMultiplierFM);
		///////////////////////

		///////////////////////
		OscillatorPhase currentPhase;
		currentPhase.set(freqAdditionPM);
		//currentPhase += osc[(_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - 1u)].getPhase();
		float val = osc[(_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - 1u)].getNextSample(currentPhase);
		///////////////////////

		if (mod[(_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - 1u)].am || (selfModOn && selfModtype == 1.f))
		{
			float selfmod = 0.f;
			if (selfModOn && selfModtype == 1.f)
			{
				selfmod = lastCarrierValue;
			}

			float cAmAmount = 0.f;
			if (mod[(_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - 1u)].am)
				cAmAmount = mod[(_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - 1u)].amAmount;
			else
				cAmAmount = mod[(_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - 1u)].amVal = 1.f;


			val *= cAmAmount * mod[(_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - 1u)].amVal + selfmod * selfModAmount;
		}
		float valueOfCarrier = val;
		lastCarrierValue = valueOfCarrier;

		buffer[sampleIndex] = Sample(valueOfCarrier);

		if (timeSinceNoteOff <= aux::millisecToSamples(attack))
			performAHDSR<Sample>(buffer, voice, timeInSamples, sampleIndex, attack, release, hold, decay, sustain, sustainOn, sustainLevel, holdLevel, lastUsedAHDSRMultiplier);
		else
			lastUsedAHDSRMultiplier = performAHDSR<Sample>(buffer, voice, timeInSamples, sampleIndex, attack, release, hold, decay, sustain, sustainOn, sustainLevel, holdLevel);
	}
}
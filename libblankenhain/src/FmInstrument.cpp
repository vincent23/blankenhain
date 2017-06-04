#include "FmInstrument.h"
#include "InterpolatedValue.h"
#include "ParameterBundle.h"
#include "FloatParameter.h"
#include "VoiceState.h"

//Todo selfmod!

FmInstrument::FmInstrument()
	: InstrumentBase(116, 1, true), currentSound(nullptr), lastCarrierValue(1.f)
{
	ParameterBundle* params = getPointerToParameterBundle();

	// envelope global
	params->getParameter(0) = new FloatParameter(50.f, NormalizedRange(1.f, 1700.f, 0.3f), "attack", "ms");
	params->getParameter(1) = new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "hold", "ms");
	params->getParameter(2) = new FloatParameter(1.f, NormalizedRange(), "holdlevel", "ratio");
	params->getParameter(3) = new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "decay", "ms");
	params->getParameter(4) = new BoolParameter(false, "sustainBool");
	params->getParameter(5) = new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "sustain", "ms");
	params->getParameter(6) = new FloatParameter(1.0f, NormalizedRange(), "sustainLevel", "ratio");
	params->getParameter(7) = new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "release", "ms");


	// Modulation Oscillators
	for (unsigned int i = 0u; i < 8; i++)
	{
		unsigned int currentIter = 8u + i * 13;
		params->getParameter(currentIter + 0) = new FloatParameter(440.f, NormalizedRange(33.f, 16000.f, 0.25), "freq", "hz");
		BhString names[3] = { "FM", "PM", "AM" };
		params->getParameter(currentIter + 1) = new OptionParameter(3u, names, "modType", "");
		params->getParameter(currentIter + 2) = new FloatParameter(0.f, NormalizedRange(), "modAmount", "amount");

		float* numbersOfOsc = new float[_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - i];
		for (unsigned int j = i; j < _LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - 1; j++)
		{
			unsigned int accessArray = j - i;
			numbersOfOsc[accessArray] = static_cast<float>(j + 1);
		}

		params->getParameter(currentIter + 3) = new DiscreteParameter(_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - i - 1, "target", "", numbersOfOsc, _LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - i - 2);
		delete[] numbersOfOsc;

		params->getParameter(currentIter + 4) = new FloatParameter(0.f, NormalizedRange(), "selfmodAmount", "amount");
		BhString waveform[4] = { "Sine", "Saw", "Square", "Triangle" };
		params->getParameter(currentIter + 5) = new OptionParameter(3u, names, "SelfmodType", "");
		params->getParameter(currentIter + 6) = new OptionParameter(4u, waveform, "waveform", "");
		params->getParameter(currentIter + 7) = new BoolParameter(false, "isOn");
		params->getParameter(currentIter + 8) = new BoolParameter(false, "isLFO");
		params->getParameter(currentIter + 9) = new BoolParameter(false, "tempoSyncOn");
		float multiplierValues[7] = { 0.0625, 0.125, 0.25, 0.5, 1., 2., 4. };
		params->getParameter(currentIter + 10) = new DiscreteParameter( 7u, "multiplier", "", multiplierValues, 4u);
		params->getParameter(currentIter + 11) = new BoolParameter(false, "TrackKey");

		float* trackKeyShiftOptions = new float[9u];
		for (unsigned int j = 0u; j < 9u; j++)
		{
			trackKeyShiftOptions[j] = static_cast<float>(-4 + static_cast<int>(j));
		}

		params->getParameter(currentIter + 12) = new DiscreteParameter(9u, "TrackKeyShift","", trackKeyShiftOptions, 4u);
		delete[] trackKeyShiftOptions;

	}

	BhString names[3] = { "FM", "PM", "AM" };
	BhString waveform[4] = { "Sine", "Saw", "Square", "Triangle" };
	//Carrier
	params->getParameter(112) = new FloatParameter(0.f, NormalizedRange(), "selfmodAmount", "amount");
	params->getParameter(113) = new OptionParameter(3u, names, "SelfmodType", "");

	params->getParameter(114) = new OptionParameter(4u, waveform, "waveform", "");

	params->getParameter(115) = new FloatParameter(0.f, NormalizedRange(0.f, 0.3f), "glide", "");

	for (unsigned int i = 0u; i < _LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH; i++)
	{
		osc[i].setMode(NaiveOscillator::NaiveOscillatorMode::OSCILLATOR_MODE_SINE);
		lastOscValues[i] = 1.f;
	}
	currentSound = &osc[_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH -1];
}

void FmInstrument::processVoice(VoiceState& voice, unsigned int timeInSamples, Sample* buffer, unsigned int numberOfSamples)
{
	float attack = getInterpolatedParameter(0).get();
	float hold = getInterpolatedParameter(1).get();
	float holdLevel = getInterpolatedParameter(2).get();
	float decay = getInterpolatedParameter(3).get();
	bool sustainOn = getInterpolatedParameter(4).get() > 0.5 ? true : false;
	float sustainLevel = getInterpolatedParameter(6).get();
	float sustain = getInterpolatedParameter(5).get();
	float release = getInterpolatedParameter(7).get();

	// Portamento stuff
	float portamento = getInterpolatedParameter(115).get();
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

	// reset modulation matrix
	for (unsigned int i = 0u; i < _LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH; i++)
		this->mod[i] = FmModulation();

	// Modulation Oscs are now evaluated
	for (unsigned int sampleIndex = 0; sampleIndex < numberOfSamples; sampleIndex++)
	{
		//unsigned int deltaT = (timeInSamples + sampleIndex) - voice.onTime;
		unsigned int timeSinceNoteOff = (timeInSamples + sampleIndex) - timeNoteOff;
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
			currentFreq = voiceFreq * (1.f - BhMath::exp(-1.f * timeSinceNoteOff / (constants::sampleRate * portamento))) +
			freqPrev * BhMath::exp(-1.f * timeSinceNoteOff / (constants::sampleRate * portamento));




		unsigned int deltaT = (timeInSamples + sampleIndex) - voice.onTime;

		for (unsigned int i = 0u; i < _LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - 1u; i++)
		{
			float amount = getInterpolatedParameter(8 + i * 13 + 2).get();
			bool isOn = getInterpolatedParameter(8 + i * 13 + 7).get() == 1.f;
			if (isOn)
			{
				float freq = getInterpolatedParameter(8 + i * 13 + 0).get();
				const float modType = getInterpolatedParameter(8 + i * 13 + 1).get();
				const float target = getInterpolatedParameter(8 + i * 13 + 3).get();
				const float selfModAmount = getInterpolatedParameter(8 + i * 13 + 4).get() * 0.0001f;
				const bool selfModOn = selfModAmount > 0.0000000001f;
				const float selfModtype = getInterpolatedParameter(8 + i * 13 + 5).get();
				const float waveFormType = getInterpolatedParameter(8 + i * 13 + 6).get();
				const bool isLFO = getInterpolatedParameter(8 + i * 13 + 8).get() == 1.f;
				const bool tempoSync = getInterpolatedParameter(8 + i * 13 + 9).get() == 1.f;
				float tempoSyncVal = getInterpolatedParameter(8 + i * 13 + 10).get();
				const bool keyFreqTrack = getInterpolatedParameter(8 + i * 13 + 11).get() == 1.f;
				const float keyFreqOctave = getInterpolatedParameter(8 + i * 13 + 12).get();


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

						float quarterNoteLength = (60.f /*seconds in a minute*/ * tempoSyncVal) / tempodata.bpm;
						float sixteenthNoteLength = quarterNoteLength / 4.f;
						float wholeBeatLength = sixteenthNoteLength * 16.f;
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
				if (mod[i].fm || (selfModOn && selfModtype == 0.f))
				{
					float selfmod = 0.f;
					if (selfModOn && selfModtype == 0.f)
					{
						selfmod = lastOscValues[i];
					}
		
					float& cFmVal = mod[i].fmVal;
					float fmAmount = 0.f;
					if (mod[i].fm)
						fmAmount = mod[i].fmAmount;

					freqMultiplierFM = cFmVal * fmAmount + selfmod * selfModAmount;
				}

				if (mod[i].pm || (selfModOn && selfModtype == 1.f))
				{
					float selfmod = 0.f;
					if (selfModOn && selfModtype == 1.f)
					{
						selfmod = lastOscValues[i];
					}

					float cPmVal = 0.f;
					if (mod[i].pm)
						cPmVal = mod[i].pmVal;

					freqAdditionPM = (mod[i].pmVal * mod[i].pmAmount) + selfmod  * selfModAmount;
				}

				osc[i].setFrequency(freq + freqMultiplierFM);

				OscillatorPhase currentPhase;
				currentPhase.set(freqAdditionPM);
				float val = osc[i].getSample(deltaT, currentPhase);

				if (mod[i].am || (selfModOn && selfModtype == 2.f))
				{
					float selfmod = 0.f;
					if (selfModOn && selfModtype == 2.f)
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
					mod[unsigned int(target)].fmVal = val;
					mod[unsigned int(target)].fmAmount = amount;
					mod[unsigned int(target)].fm = true;
				}
				else if (modType == 1.f)
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

		float freq = currentFreq;
		float selfModAmount = getInterpolatedParameter(112).get();
		bool selfModOn = selfModAmount > 1e-15;
		float selfModtype = getInterpolatedParameter(113).get();
		float waveFormType = getInterpolatedParameter(114).get();

		float freqMultiplierFM = 1.f;
		float freqAdditionPM = 0.f;

		osc[(_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - 1u)].setMode(NaiveOscillator::NaiveOscillatorMode(static_cast<int>(waveFormType)));

		if (mod[(_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - 1u)].fm || (selfModOn && selfModtype == 0.f))
		{
			float selfmod = 0.f;
			if (selfModOn && selfModtype == 0.f)
			{
				selfmod = lastCarrierValue;
			}

			float& cFmVal = mod[(_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - 1u)].fmVal;
			float fmAmount = 0.f;
			if (mod[(_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - 1u)].fm)
				fmAmount = mod[(_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - 1u)].fmAmount;

			freqMultiplierFM = ((cFmVal * fmAmount) + (selfModAmount * selfmod));
		}

		if (mod[(_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - 1u)].pm || (selfModOn && selfModtype == 1.f))
		{
			float selfmod = 0.f;
			if (selfModOn && selfModtype == 1.f)
			{
				selfmod = lastCarrierValue;
			}

			float cPmVal = 0.f;
			if (mod[(_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - 1u)].pm)
				cPmVal = mod[(_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - 1u)].pmVal;

			freqAdditionPM = (cPmVal * mod[(_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - 1u)].pmAmount) + (selfModAmount * selfmod);
			//osc[(numOsc - 1u)].setFrequency(freq + (cPmVal * mod[(numOsc - 1u)].pmAmount) + (selfModAmount * 5.f * selfmod));
		}

		///////////////////////
		osc[(_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - 1u)].setFrequency(freq + freqMultiplierFM);
		///////////////////////

		///////////////////////
		OscillatorPhase currentPhase;
		currentPhase.set(freqAdditionPM);
		float val = osc[(_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - 1u)].getSample(deltaT, currentPhase);
		///////////////////////

		if (mod[(_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH - 1u)].am || (selfModOn && selfModtype == 2.f))
		{
			float selfmod = 0.f;
			if (selfModOn && selfModtype == 2.f)
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
		performAHDSR<Sample>(buffer, voice, timeInSamples, sampleIndex, attack, release, hold, decay, sustain, sustainOn, sustainLevel, holdLevel);
	}
}
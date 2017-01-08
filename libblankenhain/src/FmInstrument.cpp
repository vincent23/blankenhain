#include "FmInstrument.h"
#include "InterpolatedValue.h"
#include "ParameterBundle.h"
#include "FloatParameter.h"
#include "VoiceState.h"

#include <cmath>

//Todo selfmod!

FmInstrument::FmInstrument()
	: InstrumentBase(99, 1, true), currentSound(nullptr)
{
	ParameterBundle* params = getPointerToParameterBundle();
	// envelope global
	params->getParameter(0) = new FloatParameter(50.f, NormalizedRange(1.f, 1700.f, 0.3f), "attack", "ms");
	params->getParameter(1) = new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "hold", "ms");
	params->getParameter(2) = new FloatParameter(1.f, NormalizedRange(), "holdlevel", "ratio");
	params->getParameter(3) = new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "decay", "ms");
	params->getParameter(4) = new FloatParameter(0.f, NormalizedRange(), "sustainBool", "bool");
	params->getParameter(5) = new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "sustain", "ms");
	params->getParameter(6) = new FloatParameter(1.0f, NormalizedRange(), "sustainLevel", "ratio");
	params->getParameter(7) = new FloatParameter(100.f, NormalizedRange(1.f, 1700.f, 0.3f), "release", "ms");

	for (unsigned int i = 8u; i < 8 * 11 + 8; i += 11)
	{
		params->getParameter(i + 0) = new FloatParameter(440.f, NormalizedRange(33.f, 16000.f, 0.25), "freq", "hz");
		params->getParameter(i + 1) = new FloatParameter(0.f, NormalizedRange(), "modType", "type");
		params->getParameter(i + 2) = new FloatParameter(0.f, NormalizedRange(), "modAmount", "amount");
		params->getParameter(i + 3) = new FloatParameter(static_cast<float>(numOsc - 1u), NormalizedRange(0.f, static_cast<float>(numOsc - 1u), 1.f), "target", "");
		params->getParameter(i + 4) = new FloatParameter(0.f, NormalizedRange(), "selfmodAmount", "amount");
		params->getParameter(i + 5) = new FloatParameter(0.f, NormalizedRange(), "selfmodType", "type");
		params->getParameter(i + 6) = new FloatParameter(0.f, NormalizedRange(), "waveform", "type");

    params->getParameter(i + 7) = new FloatParameter(0.f, NormalizedRange(), "isOn", "");
    params->getParameter(i + 8) = new FloatParameter(0.f, NormalizedRange(), "isLFO", "");
    params->getParameter(i + 9) = new FloatParameter(0.f, NormalizedRange(), "tempoSyncOn", "");
    params->getParameter(i + 10) = new FloatParameter( 1.f, NormalizedRange(0.0625f, 2.f, 0.23f), "tempoSyncMultiplier", "");


	}

  params->getParameter(96) = new FloatParameter(0.f, NormalizedRange(), "selfmodAmount", "amount");
  params->getParameter(97) = new FloatParameter(0.f, NormalizedRange(), "selfmodType", "type");
  params->getParameter(98) = new FloatParameter(0.f, NormalizedRange(), "waveform", "type");


	for (unsigned int i = 0u; i < numOsc; i++)
	{
		osc[i].setMode(NaiveOscillator::NaiveOscillatorMode::OSCILLATOR_MODE_SINE);
		lastValue[i] = 0.f;
	}
	currentSound = &osc[numOsc-1];
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
	
  // reset mod
  for (unsigned int i = 0u; i < numOsc; i++)
    this->mod[i] = FmModulation();

	// last osc is always carrier

	for (unsigned int sampleIndex = 0; sampleIndex < numberOfSamples; sampleIndex++)
	{
		unsigned int deltaT = (timeInSamples + sampleIndex) - voice.onTime;
		float valueOfCarrier = 0.f;

		for (unsigned int i = 0u; i < numOsc - 1u; i++)
		{
			float amount = getInterpolatedParameter(8 + i * 11 + 2).get();
      bool isOn = getInterpolatedParameter(8 + i * 11 + 7).get();
			if (isOn && amount != 0.f)
			{
				float freq = getInterpolatedParameter(8 + i * 11 + 0).get();
				float modType = getInterpolatedParameter(8 + i * 11 + 1).get();
				//amount
				float target = getInterpolatedParameter(8 + i * 11 + 3).get();
				//params->getParameter(i + 4) = new FloatParameter(0.f, NormalizedRange(), "selfmod", "amount");
				//params->getParameter(i + 5) = new FloatParameter(0.f, NormalizedRange(), "selfmod", "type");
				float waveFormType = getInterpolatedParameter(8 + i * 11 + 6).get() * 4.f;
        bool isLFO = getInterpolatedParameter(8 + i * 11 + 8).get();
        bool tempoSync = getInterpolatedParameter(8 + i * 11 + 9).get();
        float tempoSyncVal = getInterpolatedParameter(8 + i * 11 + 10).get();

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

				osc[i].setMode(NaiveOscillator::NaiveOscillatorMode(static_cast<int>(waveFormType)));

				if (mod[i].fm)
				{
          float cFmVal = mod[i].fmVal + 1.f;
					osc[i].setFrequency(freq * (cFmVal * mod[i].fmAmount));
				}
				else if (mod[i].pm)
				{
					osc[i].setFrequency(freq + (mod[i].pmVal * mod[i].pmAmount));
				}
				else
				{
					osc[i].setFrequency(freq);
				}

				float val = osc[i].getSample(deltaT);
				if (mod[i].am)
				{
					val *= mod[i].amAmount * mod[i].amVal;
				}

				if (modType < 0.333)
				{
					mod[unsigned int(target)].fmVal = val;
					mod[unsigned int(target)].fmAmount = amount;
					mod[unsigned int(target)].fm = true;
				}
				else if (modType < 0.6666)
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


		float freq = aux::noteToFrequency(voice.key);
		//params->getParameter(i + 4) = new FloatParameter(0.f, NormalizedRange(), "selfmod", "amount");
		//params->getParameter(i + 5) = new FloatParameter(0.f, NormalizedRange(), "selfmod", "type");
		float waveFormType = getInterpolatedParameter(98).get() * 4.f;

		osc[(numOsc - 1u)].setMode(NaiveOscillator::NaiveOscillatorMode(static_cast<int>(waveFormType)));

		if (mod[(numOsc - 1u)].fm)
		{
			// FM is currently pm
			osc[(numOsc - 1u)].setFrequency(freq * ((mod[(numOsc - 1u)].fmVal + 1.f) * mod[(numOsc - 1u)].fmAmount));
		}
		else if (mod[(numOsc - 1u)].pm)
		{
			osc[(numOsc - 1u)].setFrequency(freq + (mod[(numOsc - 1u)].pmVal * mod[(numOsc - 1u)].pmAmount));
		}
		else
		{
			osc[(numOsc - 1u)].setFrequency(freq);
		}

		float val = osc[(numOsc - 1u)].getSample(deltaT);
		if (mod[(numOsc - 1u)].am)
		{
			val *= mod[(numOsc - 1u)].amAmount * mod[(numOsc - 1u)].amVal;
		}
		valueOfCarrier = val;

		buffer[sampleIndex] = Sample(valueOfCarrier);
		performAHDSR<Sample>(buffer, voice, timeInSamples, sampleIndex, attack, release, hold, decay, sustain, sustainOn, sustainLevel, holdLevel);
	}
	this->nextSample(numberOfSamples);
}
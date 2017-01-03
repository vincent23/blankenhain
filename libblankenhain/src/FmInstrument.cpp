#include "FmInstrument.h"
#include "InterpolatedValue.h"
#include "ParameterBundle.h"
#include "FloatParameter.h"
#include "VoiceState.h"

#include <cmath>

FmInstrument::FmInstrument()
	: InstrumentBase(8 + 10*7, 1), currentSound(nullptr)
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

	// carrier
	//params->getParameter(8) = new FloatParameter(0.f, NormalizedRange(), "selfmod", "amount");
	//params->getParameter(9) = new FloatParameter(0.f, NormalizedRange(), "selfmod", "type");
	//params->getParameter(10) = new FloatParameter(0.f, NormalizedRange(), "waveform", "type");

	for (unsigned int i = 8u; i < 10 * 7 + 8; i += 7)
	{
		params->getParameter(i + 0) = new FloatParameter(440.f, NormalizedRange(33.f, 16000.f, 0.25), "freq", "hz");
		params->getParameter(i + 1) = new FloatParameter(0.f, NormalizedRange(), "mod", "type");
		params->getParameter(i + 2) = new FloatParameter(0.f, NormalizedRange(), "mod", "amount");
		params->getParameter(i + 3) = new FloatParameter(9.f, NormalizedRange(0.f, 9.f, 1.f), "target", "");
		params->getParameter(i + 4) = new FloatParameter(0.f, NormalizedRange(), "selfmod", "amount");
		params->getParameter(i + 5) = new FloatParameter(0.f, NormalizedRange(), "selfmod", "type");
		params->getParameter(i + 6) = new FloatParameter(0.f, NormalizedRange(), "waveform", "type");
	}

	for (unsigned int i = 0u; i < numOsc; i++)
	{
		osc[i].setMode(NaiveOscillator::NaiveOscillatorMode::OSCILLATOR_MODE_SINE);
		lastValue[i] = 0.f;
	}
	currentSound = &osc[9];
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
	
	// carrier
	//this->osc[0u].setFrequency(aux::noteToFrequency(voice.key));
	//float carrierSelfMod = getInterpolatedParameter(8).get();
	//// 0 - 1/3: FM || 1/3 - 2/3: PM || 2/3 - 1: AM
	//float carrierSelfModType = getInterpolatedParameter(9).get();
	//float carrierSelfModWaveform = getInterpolatedParameter(10).get();
	//this->osc[0u].setMode(NaiveOscillator::NaiveOscillatorMode(static_cast<int>(carrierSelfModWaveform * 4.f)));

	for (unsigned int sampleIndex = 0; sampleIndex < numberOfSamples; sampleIndex++)
	{
		unsigned int deltaT = (timeInSamples + sampleIndex) - voice.onTime;
		float valueOfCarrier = 0.f;

		for (unsigned int i = 0u; i < numOsc - 1u; i++)
		{
			float amount = getInterpolatedParameter(8 + i * 7 + 2).get();
			if (amount != 0.f)
			{
				float freq = getInterpolatedParameter(8 + i * 7 + 0).get();
				float modType = getInterpolatedParameter(8 + i * 7 + 1).get();
				//amount
				float target = getInterpolatedParameter(8 + i * 7 + 3).get();
				//params->getParameter(i + 4) = new FloatParameter(0.f, NormalizedRange(), "selfmod", "amount");
				//params->getParameter(i + 5) = new FloatParameter(0.f, NormalizedRange(), "selfmod", "type");
				float waveFormType = getInterpolatedParameter(8 + i * 7 + 6).get() * 4.f;
			
				osc[i].setMode(NaiveOscillator::NaiveOscillatorMode(static_cast<int>(waveFormType)));

				if (mod[i].fm)
				{
					// FM is currently pm
					osc[i].setFrequency(freq + (mod[i].fmVal * mod[i].fmAmount));
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
		float waveFormType = getInterpolatedParameter(8 + (numOsc - 1u) * 7 + 6).get() * 4.f;

		osc[(numOsc - 1u)].setMode(NaiveOscillator::NaiveOscillatorMode(static_cast<int>(waveFormType)));

		if (mod[(numOsc - 1u)].fm)
		{
			// FM is currently pm
			osc[(numOsc - 1u)].setFrequency(freq + (mod[(numOsc - 1u)].fmVal * mod[(numOsc - 1u)].fmAmount));
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
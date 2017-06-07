#include "VolumeEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"

VolumeEffect::VolumeEffect() : EffectBase(10u, true)
{
	ParameterBundle& params = getParameterBundle();
	params.getParameter(0) = new FloatParameter(0.f, NormalizedRange(-120.f, 12.f, 5.f), "volumeL", "dB");
	params.getParameter(1) = new FloatParameter(0.f, NormalizedRange(-120.f, 12.f, 5.f), "volumeR", "dB");
	params.getParameter(2) = new FloatParameter(1.f, NormalizedRange(), "coupling", "bool");
	BhString names[4] = { "sine", "saw", "square", "triangle" };
	params.getParameter(3) = new FloatParameter(0.f, NormalizedRange(-1.f, 1.f), "lfoAmount", "");
	params.getParameter(4) = new FloatParameter(0.0055f, NormalizedRange(0.005f, 20.f, 0.325f), "lfoSpeed", "");
	float multiplierValues[7] = { 0.0625, 0.125, 0.25, 0.5, 1., 2., 4. };
	params.getParameter(5) = new DiscreteParameter(7u, "lfoBeatMultiplier", "", multiplierValues);
	params.getParameter(6) = new OptionParameter(4u, names, "lfoWaveform", "");
	params.getParameter(7) = new BoolParameter(false, "lfoTemposync");
	params.getParameter(8) = new FloatParameter(0.f, NormalizedRange(0.f, 2.f * 3.14159265359f), "lfoPhase", "");
	params.getParameter(9) = new FloatParameter(0.f, NormalizedRange(-1.f, 1.f), "lfoBaseline", "");
}

void VolumeEffect::process(Sample* buffer, size_t numberOfSamples, size_t currentTime)
{
	float currentVolumeL = interpolatedParameters.get(0);
	float currentVolumeR = interpolatedParameters.get(1);
	bool coupling = interpolatedParameters.get(2) > 0.5 ? true : false;


	if (coupling)
	{
		float decibelBefore = aux::decibelToLinear(currentVolumeL);
		float decibelAfter = aux::decibelToLinear(currentVolumeL);
		InterpolatedValue<float> volumeInDecibel(decibelBefore, decibelAfter, numberOfSamples);
		for (size_t bufferIteration = 0; bufferIteration < numberOfSamples; bufferIteration++)
		{
			buffer[bufferIteration] *= Sample(volumeInDecibel.get());
			volumeInDecibel.next();
		}
	}
	else
	{
		float decibelBeforeL = aux::decibelToLinear(currentVolumeL);
		float decibelBeforeR = aux::decibelToLinear(currentVolumeR);
		float decibelAfterL = aux::decibelToLinear(currentVolumeL);
		float decibelAfterR = aux::decibelToLinear(currentVolumeR);
		InterpolatedValue<float> volumeInDecibelL(decibelBeforeL, decibelAfterL, numberOfSamples);
		InterpolatedValue<float> volumeInDecibelR(decibelBeforeR, decibelAfterR, numberOfSamples);
		for (size_t bufferIteration = 0u; bufferIteration < numberOfSamples; bufferIteration++)
		{
			buffer[bufferIteration] *= Sample(volumeInDecibelL.get(), volumeInDecibelR.get());
			volumeInDecibelL.next();
			volumeInDecibelR.next();
		}
	}
}

void VolumeEffect::getModulation(float* modulationValues, size_t sampleOffset)
{
	float lfoAmount = interpolatedParameters.get(3);

	// Perform LFO on volumeL
	if (lfoAmount != 0.f)
	{
		float lfoBaseline = interpolatedParameters.get(9);

		float lfoWaveform = interpolatedParameters.get(6);
		bool lfoTempoSync = interpolatedParameters.get(7) == 1.f;
		this->lfo.setMode(NaiveOscillator::NaiveOscillatorMode(static_cast<unsigned int>(lfoWaveform)));
		float lfoPhase = interpolatedParameters.get(8);
		if (!lfoTempoSync)
		{
			float lfoSpeed = interpolatedParameters.get(4);
			this->lfo.setFrequency(lfoSpeed);
		}
		else
		{
			float lfoMult = interpolatedParameters.get(5);
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

		for (unsigned int i = 0u; i < sampleOffset; i++)
		{
			if (this->effectUsesTempoData())
				modulationValues[0] = this->lfo.getSample(i + this->tempodata.position, lfoPhase) * lfoAmount + lfoBaseline;
			else
				modulationValues[0] = this->lfo.getNextSample(lfoPhase) * lfoAmount + lfoBaseline;
		}
	}
}
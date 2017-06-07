#include "WidthEffect.h"

// Change the name and define parameters in constructor
WidthEffect::WidthEffect() : EffectBase(9u, true), lfo()
{
	ParameterBundle& params = getParameterBundle();
	// Insert your stuff here
	params.getParameter(0) = new FloatParameter(1.f, NormalizedRange(0.f, 2.f), "width", "");
	params.getParameter(1) = new FloatParameter(0.f, NormalizedRange(-50.f, 50.f), "pan", "");

	// LFO for pan
	BhString names[4] = { "sine", "saw", "square", "triangle" };
	params.getParameter(2) = new FloatParameter(0.f, NormalizedRange(-1.f, 1.f), "lfoAmount", "");
	params.getParameter(3) = new FloatParameter(0.0055f, NormalizedRange(0.005f, 20.f, 0.325f), "lfoSpeed", "");
	float multiplierValues[7] = { 0.0625, 0.125, 0.25, 0.5, 1., 2., 4. };
	params.getParameter(4) = new DiscreteParameter(7u, "lfoBeatMultiplier", "", multiplierValues);
	params.getParameter(5) = new OptionParameter(4u, names, "lfoWaveform", "");
	params.getParameter(6) = new BoolParameter(false, "lfoTemposync");
	params.getParameter(7) = new FloatParameter(0.f, NormalizedRange(0.f, 2.f * 3.14159265359f), "lfoPhase", "");
	params.getParameter(8) = new FloatParameter(0.f, NormalizedRange(-1.f, 1.f), "lfoBaseline", "");
}

void WidthEffect::process(Sample* buffer, size_t numberOfSamples, size_t currentTime)
{
	float width = interpolatedParameters.get(0);
	float panningValue = interpolatedParameters.get(1);

	for (size_t i = 0; i < numberOfSamples; i++)
	{
		Sample in = buffer[i];
		Sample inFlipped = in.flippedChannels();
		Sample widthAsSample = Sample(width);
		Sample avg = in * Sample(.5) + inFlipped * Sample(.5);
		buffer[i] = avg * (Sample(1) - widthAsSample) + in * widthAsSample;

		float panValue = panningValue * .02f;
		aux::performPanning(buffer[i], panValue);
	}
}

void WidthEffect::getModulation(float* modulationValues, size_t sampleOffset)
{
	float lfoAmount = interpolatedParameters.get(2);

	// Perform LFO on volumeL
	if (lfoAmount != 0.f)
	{
		float lfoBaseline = interpolatedParameters.get(8);

		float lfoWaveform = interpolatedParameters.get(5);
		bool lfoTempoSync = interpolatedParameters.get(6) == 1.f;
		this->lfo.setMode(NaiveOscillator::NaiveOscillatorMode(static_cast<unsigned int>(lfoWaveform)));
		float lfoPhase = interpolatedParameters.get(7);
		if (!lfoTempoSync)
		{
			float lfoSpeed = interpolatedParameters.get(3);
			this->lfo.setFrequency(lfoSpeed);
		}
		else
		{
			float lfoMult = interpolatedParameters.get(4);
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
				modulationValues[1] = this->lfo.getSample(i + this->tempodata.position, lfoPhase) * lfoAmount + lfoBaseline;
			else
				modulationValues[1] = this->lfo.getNextSample(lfoPhase) * lfoAmount + lfoBaseline;
		}
	}
}
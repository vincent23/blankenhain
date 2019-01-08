#include "FilterEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "BhMath.h"

const unsigned int NUMBER_OF_PARAMETERS = 11u;

FilterEffect::FilterEffect() : EffectBase(NUMBER_OF_PARAMETERS, true)
{
	ParameterBundle& params = getParameterBundle();
	// Insert your stuff here
	params.initParameter(0, new FloatParameter(0.7f, NormalizedRange::fromMidpoint(0.1f, 0.7f, 10.f), "Q", ""));
	params.initParameter(1, new FloatParameter(120.f, NormalizedRange::fromMidpoint(20.f, 850.f, 22000.f), "frequency", "Hz"));
	BhString filterStyles[4] = { "HighPass", "LowPass", "BandPass", "Notch" };
	params.initParameter(2,new OptionParameter(4u, filterStyles, "FilterStyles", ""));
	params.initParameter(3,new FloatParameter(1.01f, NormalizedRange(0.f, 2.9999f, 1.f), "rolloff", ""));

	params.initParameter(4,new FloatParameter(0.f, NormalizedRange(0.f, 0.25f), "lfoAmount", ""));
	params.initParameter(5,new FloatParameter(0.0055f, NormalizedRange(0.005f, 20.f, 0.325f), "lfoSpeed", ""));
	float multiplierValues[7] = { 0.0625, 0.125, 0.25, 0.5, 1., 2., 4. };
	params.initParameter(6,new DiscreteParameter(7u, "lfoBeatMultiplier", "", multiplierValues));
	BhString names[4] = { "sine", "saw", "square", "triangle" };
	params.initParameter(7,new OptionParameter(4u, names, "lfoWaveform", ""));
	params.initParameter(8,new BoolParameter(false, "lfoTemposync"));
	params.initParameter(9,new FloatParameter(0.f, NormalizedRange(0.f, 2.f * 3.14159265359f), "lfoPhase", ""));
	params.initParameter(10,new FloatParameter(0.f, NormalizedRange(-1.f, 1.f), "lfoBaseline", ""));

	//resonance = new FloatParameter(0.5, "Resonance", 0.5f, NormalizedRange(0.f, 5.f));
	//frequency = new FloatParameter((500.f), "Frequency", 0.5f, NormalizedRange(40.f, 22000.f, 0.3f));
}

void FilterEffect::process(Sample* buffer, size_t numberOfSamples, size_t currentTime)
{
	float Q = interpolatedParameters.get(0);
	float frequency = interpolatedParameters.get(1);
	float style = interpolatedParameters.get(2);
	int numberOfFilters = static_cast<int>(interpolatedParameters.get(3) + 1);
	if (numberOfFilters == 3) {
		// 3 means 4
		numberOfFilters = 4;
	}
	// remap Q
	Q = BhMath::pow(Q, 1.f/ static_cast<float>(numberOfFilters));

	for (unsigned int filterIndex = 0; static_cast<int>(filterIndex) < numberOfFilters; filterIndex++) {
		Filter<Sample>& filter = filters[filterIndex];
		if (style < 0.5f) {
			filter.setHighPass(frequency, Q);
		}
		else if (style < 1.5f) {
			filter.setLowPass(frequency, Q);
		}
		else if (style < 2.5f) {
			filter.setBandPass(frequency, Q);
		}
		else {
			filter.setNotch(frequency, Q);
		}
		for (unsigned int i = 0; i < numberOfSamples; i++) {
			buffer[i] = filter.tick(buffer[i]);
		}
	}
}

void FilterEffect::getModulation(float* modulationValues, size_t sampleOffset)
{
	float lfoAmount = interpolatedParameters.get(4);

	// Perform LFO on Frequency
	if (lfoAmount != 0.f)
	{
		float lfoBaseline = interpolatedParameters.get(10);

		float lfoWaveform = interpolatedParameters.get(7);
		bool lfoTempoSync = interpolatedParameters.get(8) == 1.f;
		this->lfo.setMode(NaiveOscillatorMode(static_cast<unsigned int>(lfoWaveform)));
		float lfoPhase = interpolatedParameters.get(9);
		if (!lfoTempoSync)
		{
			float lfoSpeed = interpolatedParameters.get(5);
			this->lfo.setFrequency(lfoSpeed);
		}
		else
		{
			float lfoMult = interpolatedParameters.get(6);
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


		this->lfo.setParams(lfoBaseline, lfoAmount);
		for (unsigned int i = 0u; i < sampleOffset; i++)
		{
			if (this->effectUsesTempoData())
			{
				modulationValues[1] = this->lfo.getSample(i + this->tempodata.position,lfoPhase);
			}
			else
			{
				modulationValues[1] = this->lfo.getNextSample(lfoPhase);
			}
		}
	}
}
#pragma once
#include "Oscillators.h"
#include "Filter.h"
#include "AuxFunc.h"
#include "ModulationEnvelope.h"

// via https://www.cim.mcgill.ca/~clark/nordmodularbook/nm_percussion.html
// see also
// http://www.nordkeyboards.com/sites/default/files/files/downloads/manuals/nord-modular/Nord%20Modular%20English%20User%20Manual%20v3.0%20Edition%203.0.pdf
class BassDrumSound1 : public I_SoundGenerator
{
private:
	PulseSound pulse;
	Filter<float> filter;
public:
	BassDrumSound1(float lowPassFreq = 58.f, float lowPassQ = 5., float pulseTimeInMS = 1.f) : 
		pulse(static_cast<unsigned int>(aux::millisecToSamples(pulseTimeInMS))), filter()
	{
		filter.setLowPass(lowPassFreq, lowPassQ);
	}
	virtual float getSample(unsigned int time) final
	{
		float value = pulse.getSample(time);
		return filter.tick(value);
	}

	virtual void setFrequency(float frequency) final {};

	void setLowPass(float freq, float q)
	{
		filter.setLowPass(58.f, 5.);
	}
	void setPuleLength(float inMs)
	{
		pulse = PulseSound(static_cast<unsigned int>(aux::millisecToSamples(inMs)));
	}
	virtual float getNextSample() final
	{
		return 0;
	}
};

/** 
 * Primitive Bass drum 
 * via https://www.reddit.com/r/edmproduction/comments/1qbt1g/synthesize_your_own_drums_today/
 */
class BassDrumSound2 : public I_SoundGenerator
{
private:
	NaiveOscillator sine;
	ModulationEnvelope env;
	float baseFreq;
	float envMult;
public:
	BassDrumSound2(float envRelease, float baseFrequency, float envMultFactor) : sine(NaiveOscillator::NaiveOscillatorMode::OSCILLATOR_MODE_SINE, baseFrequency),
		env(0.f, envRelease)
	{
		baseFreq = baseFrequency;
		envMult = envMultFactor;
		//filter.setLowPass(58.f, 5.);
	}
	virtual void setFrequency(float frequency) final 
	{
		baseFreq = frequency;
	};
	virtual float getSample(unsigned int time) final
	{
		float envValue = env.get(time);
		float curFreq = baseFreq - baseFreq * (1.f - envValue) * envMult;
		if (curFreq > 30.f)
		{
			sine.setFrequency(baseFreq - baseFreq * (1.f - envValue) * envMult);
			return sine.getSample(time);
		}
		else
			return 0.f;
	}

	void setEnvMult(float envMultFactor_)
	{
		envMult = envMultFactor_;
	}
	void setEnvRelease(float envReleaseTimeInMs_)
	{
		this->env.releaseInMs = envReleaseTimeInMs_;
	}
	virtual float getNextSample() final
	{
		return 0;
	}
};

// via https://www.cim.mcgill.ca/~clark/nordmodularbook/nm_percussion.html
//
// I didnt finish this one yet
//
//class BassDrumSound3 : public I_SoundGenerator
//{
//private:
//	PulseSound pulse;
//	NaiveOscillator sineA, sineE;
//	WaveTableOscillator sawtooth;
//public:
//	BassDrumSound3() :
//		sawtooth(AdditiveSawtoothWaveOscillator()),
//		pulse(aux::millisecToSamples(1.f)), 
//		sineA(NaiveOscillator::NaiveOscillatorMode::OSCILLATOR_MODE_SINE, 49.f),
//		sineE()
//	{
//	}
//	virtual void setFrequency(float frequency, float detune) final {};
//	virtual float getSample(unsigned int time) final
//	{
//		float value1 = pulse.getSample(time);
//		float exponentialDecay = exp(-10.f * time);
//		sineA.setFrequency(49.f + exponentialDecay * 3.5 * 49.f);
//		float value2 = sineA.getSample(time);
//		sawtooth.setFrequency((49.f + exponentialDecay * 3.5 * 49.f) * 5.0397);
//		return 0.f;
//	}
//};
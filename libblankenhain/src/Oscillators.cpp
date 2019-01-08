// via http://www.martin-finke.de/blog/articles/audio-plugins-018-polyblep-oscillator/
//  Oscillator.cpp
//  SpaceBass
//
//  Created by Martin on 08.04.14.
// Heavily modified


#include "Oscillators.h"


float naiveWaveformForMode(NaiveOscillatorMode const& mode, OscillatorPhase const& phaseIn_)
{
	float value;
	const float currentPhase = phaseIn_.getValue();
	switch (mode) {
	case OSCILLATOR_MODE_SINE:
	{
		value = BhMath::sin(currentPhase);
		break;
	}
	case OSCILLATOR_MODE_SAW:
	{
		value = (currentPhase / constants::pi) - 1.f;
		break;
	}
	case OSCILLATOR_MODE_SQUARE:
	{
		if (currentPhase < (constants::pi))
		{
			value = 1.f;
		}
		else
		{
			value = -1.f;
		}
		break;
	}
	case OSCILLATOR_MODE_TRIANGLE:
	{
		const float tempvalue = -1.f + (currentPhase / (constants::pi));
		value = 2.f * (BhMath::abs(tempvalue) - 0.5f);
		break;
	}
	default:
		break;
	}
	return value;
}


NaiveOscillator::NaiveOscillator() :
	BaseOscillator(),
	mOscillatorMode(OSCILLATOR_MODE_SINE)
{
};

BaseOscillator::BaseOscillator() :
	mFrequency(440.0),
	mPhase(0.f)
{
	updateIncrement();
};

void NaiveOscillator::setMode(NaiveOscillatorMode mode) {
    mOscillatorMode = mode;
}

void BaseOscillator::setFrequency(float frequency) 
{
	mFrequency = frequency;
    updateIncrement();
}

float NaiveOscillator::getSample(unsigned int time, OscillatorPhase phase) const
{
	OscillatorPhase curPhase = mPhase;
	curPhase.set(static_cast<float>(time) * mPhaseIncrement);
	const float value = naiveWaveformForMode(mOscillatorMode, curPhase + phase);
	return value;
};

void BaseOscillator::updateIncrement() {
	mPhaseIncrement = mFrequency * (2.f * constants::pi) / constants::sampleRate;
}


OscillatorPhase operator+(const OscillatorPhase &c1, const OscillatorPhase &c2)
{
	OscillatorPhase out(c1);
	out.incrementBy(c2.getValue());
	return out;
}

float NaiveOscillator::getNextSample(OscillatorPhase phase)
{
	mPhase.incrementBy(mPhaseIncrement);
	float value = naiveWaveformForMode(mOscillatorMode, mPhase + phase);
	return value;
}

float PolyBLEPOscillator::poly_blep(float t) const
{
	float dt = mPhaseIncrement / (2.f * constants::pi);
	// 0 <= t < 1
	if (t < dt)
	{
		t /= dt;
		return t + t - t*t - 1.f;
	}
	// -1 < t < 0
	else if (t > 1.f - dt)
	{
		t = (t - 1.f) / dt;
		return t*t + t + t + 1.f;
	}
	// 0 otherwise
	else
	{
		return 0.f;
	}
}

float PolyBLEPOscillator::getSample(unsigned int time, OscillatorPhase phase)
{
	float value = 0.0;

	mPhase.set(static_cast<float>(time) * mPhaseIncrement);

	float t = BhMath::fmod((mPhase.getValue() + phase.getValue()), (2.f * constants::pi)) / (2.f * constants::pi);

	if (mOscillatorMode == OSCILLATOR_MODE_SINE) {
		value = naiveWaveformForMode(OSCILLATOR_MODE_SINE, mPhase+phase);
	}
	else if (mOscillatorMode == OSCILLATOR_MODE_SAW) {
		value = naiveWaveformForMode(OSCILLATOR_MODE_SAW, mPhase + phase);
		value -= poly_blep(t);
	}
	else {
		value = naiveWaveformForMode(OSCILLATOR_MODE_SQUARE, mPhase + phase);
		value += poly_blep(t);
		float temp = BhMath::fmod(t + 0.5f, 1.f);
		value -= poly_blep(temp);

		if (mOscillatorMode == OSCILLATOR_MODE_TRIANGLE) {
			// Leaky integrator: y[n] = A * x[n] + (1 - A) * y[n-1]
			value = mPhaseIncrement * value + (1.f - mPhaseIncrement) * lastOutput;
			lastOutput = value;
		}
	}

#ifdef _LIBBLANKENHAIN_ENABLE_NANCHECK
	if (value != value
		|| mPhase.getValue() != mPhase.getValue()
		|| t != t)
		throw "nan detected";
#endif

	return value;
}

float PolyBLEPOscillator::getNextSample(OscillatorPhase phase)
{
	float value = 0.0;

	mPhase.incrementBy(mPhaseIncrement);

	float t = BhMath::fmod((mPhase.getValue() + phase.getValue()), (2.f * constants::pi)) / (2.f * constants::pi);

	if (mOscillatorMode == OSCILLATOR_MODE_SINE) {
		value = naiveWaveformForMode(OSCILLATOR_MODE_SINE, mPhase + phase);
	}
	else if (mOscillatorMode == OSCILLATOR_MODE_SAW) {
		value = naiveWaveformForMode(OSCILLATOR_MODE_SAW, mPhase + phase);
		value -= poly_blep(t);
	}
	else 
	{
		value = naiveWaveformForMode(OSCILLATOR_MODE_SQUARE, mPhase + phase);
		value += poly_blep(t);
		const float temp = BhMath::fmod(t + 0.5f, 1.f);
		value -= poly_blep(temp);
		if (mOscillatorMode == OSCILLATOR_MODE_TRIANGLE)
		{
			// Leaky integrator: y[n] = A * x[n] + (1 - A) * y[n-1]
			value = mPhaseIncrement * value + (1.f - mPhaseIncrement) * lastOutput;
			lastOutput = value;
		}
	}
#ifndef _LIBBLANKENHAIN_ENABLE_NANCHECK
//#define _LIBBLANKENHAIN_ENABLE_NANCHECK
#endif
#ifdef _LIBBLANKENHAIN_ENABLE_NANCHECK
	if (value != value
		|| mPhase.getValue() != mPhase.getValue()
		|| t != t)
		throw "nan detected";
#endif
	return value;
}
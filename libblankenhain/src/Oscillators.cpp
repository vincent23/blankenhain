//
// via http://www.martin-finke.de/blog/articles/audio-plugins-018-polyblep-oscillator/
//  Oscillator.cpp
//  SpaceBass
//
//  Created by Martin on 08.04.14.
//
//

#include "Oscillators.h"

const float OscillatorPhase::length = 2.f * static_cast<float>(constants::pi);

NaiveOscillator::NaiveOscillator() :
	BaseOscillator(),
	mOscillatorMode(OSCILLATOR_MODE_SINE)
{
};

BaseOscillator::BaseOscillator() :
	mFrequency(440.0),
	mPhase()
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

float NaiveOscillator::getSample(unsigned int time, float phase)
{
	mPhase.set(static_cast<float>(time) * mPhaseIncrement);
	float value = naiveWaveformForMode(mOscillatorMode, phase);
	return value;
};

void BaseOscillator::updateIncrement() {
	mPhaseIncrement = mFrequency * (2.f * constants::pi) / constants::sampleRate;
}

float NaiveOscillator::naiveWaveformForMode(NaiveOscillatorMode mode, float phase) 
{
	float value;
    switch (mode) {
        case OSCILLATOR_MODE_SINE:
            value = sin(mPhase.getValue() + phase);
            break;
        case OSCILLATOR_MODE_SAW:
            value = (fmod(mPhase.getValue() + phase, 2.f * (constants::pi)) / constants::pi) - 1.0;
            break;
        case OSCILLATOR_MODE_SQUARE:
            if (fmod(mPhase.getValue() + phase, 2.f * (constants::pi)) < (constants::pi)) {
                value = 1.0;
            } else {
                value = -1.0;
            }
            break;
        case OSCILLATOR_MODE_TRIANGLE:
		{
			float tempvalue = -1.0 + (fmod(mPhase.getValue() + phase, 2.f * (constants::pi)) / (constants::pi));
			value = 2.0 * (fabs(tempvalue) - 0.5);
			break;
		}
        default:
            break;
    }
    return value;
}

float NaiveOscillator::getNextSample(float phase)
{
	mPhase.incrementBy(mPhaseIncrement);
	float value = naiveWaveformForMode(mOscillatorMode, phase);
	return value;
}

float PolyBLEPOscillator::poly_blep(float t) const
{
	float dt = mPhaseIncrement / (2. * constants::pi);
	// 0 <= t < 1
	if (t < dt)
	{
		t /= dt;
		return t + t - t*t - 1.0;
	}
	// -1 < t < 0
	else if (t > 1.0 - dt)
	{
		t = (t - 1.0) / dt;
		return t*t + t + t + 1.0;
	}
	// 0 otherwise
	else
	{
		return 0.0;
	}
}

float PolyBLEPOscillator::getSample(unsigned int time, float phase)
{
	float value = 0.0;

	mPhase.set(static_cast<float>(time) * mPhaseIncrement);

	float t = fmod((mPhase.getValue() + phase), (2.f * constants::pi)) / (2.f * constants::pi) ;

	if (mOscillatorMode == OSCILLATOR_MODE_SINE) {
		value = naiveWaveformForMode(OSCILLATOR_MODE_SINE, phase);
	}
	else if (mOscillatorMode == OSCILLATOR_MODE_SAW) {
		value = naiveWaveformForMode(OSCILLATOR_MODE_SAW, phase);
		value -= poly_blep(t);
	}
	else {
		value = naiveWaveformForMode(OSCILLATOR_MODE_SQUARE, phase);
		value += poly_blep(t);
		float temp = fmod(t + 0.5f, 1.f);
		value -= poly_blep(temp);
		if (mOscillatorMode == OSCILLATOR_MODE_TRIANGLE, phase) {
			// Leaky integrator: y[n] = A * x[n] + (1 - A) * y[n-1]
			value = mPhaseIncrement * value + (1. - mPhaseIncrement) * lastOutput;
			lastOutput = value;
		}
	}
	return value;
}

float PolyBLEPOscillator::getNextSample(float phase)
{
	float value = 0.0;

	mPhase.incrementBy(mPhaseIncrement);

	float t = fmod((mPhase.getValue() + phase ), (2.f * constants::pi)) / (2.f * constants::pi);

	if (mOscillatorMode == OSCILLATOR_MODE_SINE) {
		value = naiveWaveformForMode(OSCILLATOR_MODE_SINE, phase);
	}
	else if (mOscillatorMode == OSCILLATOR_MODE_SAW) {
		value = naiveWaveformForMode(OSCILLATOR_MODE_SAW, phase);
		value -= poly_blep(t);
	}
	else {
		value = naiveWaveformForMode(OSCILLATOR_MODE_SQUARE, phase);
		value += poly_blep(t);
		float temp = fmod(t + 0.5f, 1.f);
		value -= poly_blep(temp);
		if (mOscillatorMode == OSCILLATOR_MODE_TRIANGLE)
		{
			// Leaky integrator: y[n] = A * x[n] + (1 - A) * y[n-1]
			value = mPhaseIncrement * value + (1. - mPhaseIncrement) * lastOutput;
			lastOutput = value;
		}
	}
	return value;
}
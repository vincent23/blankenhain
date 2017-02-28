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

float NaiveOscillator::getSample(unsigned int time, OscillatorPhase phase)
{
	mPhase.set(static_cast<float>(time) * mPhaseIncrement);
	float value = naiveWaveformForMode(mOscillatorMode, phase.getValue());
	return value;
};

void BaseOscillator::updateIncrement() {
	mPhaseIncrement = mFrequency * (2.f * constants::pi) / constants::sampleRate;
}

float NaiveOscillator::naiveWaveformForMode(NaiveOscillatorMode mode, OscillatorPhase phase)
{
	float value;
    switch (mode) {
        case OSCILLATOR_MODE_SINE:
            value = BhMath::sin(mPhase.getValue() + phase.getValue());
            break;
        case OSCILLATOR_MODE_SAW:
            value = (BhMath::fmod(mPhase.getValue() + phase.getValue(), 2.f * (constants::pi)) / constants::pi) - 1.f;
            break;
        case OSCILLATOR_MODE_SQUARE:
            if (BhMath::fmod(mPhase.getValue() + phase.getValue(), 2.f * (constants::pi)) < (constants::pi)) {
                value = 1.f;
            } else {
                value = -1.f;
            }
            break;
        case OSCILLATOR_MODE_TRIANGLE:
		{
			float tempvalue = -1.f + (BhMath::fmod(mPhase.getValue() + phase.getValue(), 2.f * (constants::pi)) / (constants::pi));
			value = 2.f * (BhMath::abs(tempvalue) - 0.5f);
			break;
		}
        default:
            break;
    }
    return value;
}

float NaiveOscillator::getNextSample(OscillatorPhase phase)
{
	mPhase.incrementBy(mPhaseIncrement);
	float value = naiveWaveformForMode(mOscillatorMode, phase.getValue());
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
		value = naiveWaveformForMode(OSCILLATOR_MODE_SINE, phase.getValue());
	}
	else if (mOscillatorMode == OSCILLATOR_MODE_SAW) {
		value = naiveWaveformForMode(OSCILLATOR_MODE_SAW, phase.getValue());
		value -= poly_blep(t);
	}
	else {
		value = naiveWaveformForMode(OSCILLATOR_MODE_SQUARE, phase.getValue());
		value += poly_blep(t);
		float temp = BhMath::fmod(t + 0.5f, 1.f);
		value -= poly_blep(temp);
		if (mOscillatorMode == OSCILLATOR_MODE_TRIANGLE) {
			// Leaky integrator: y[n] = A * x[n] + (1 - A) * y[n-1]
			value = mPhaseIncrement * value + (1.f - mPhaseIncrement) * lastOutput;
			lastOutput = value;
		}
	}
	return value;
}

float PolyBLEPOscillator::getNextSample(OscillatorPhase phase)
{
	float value = 0.0;

	mPhase.incrementBy(mPhaseIncrement);

	float t = BhMath::fmod((mPhase.getValue() + phase.getValue()), (2.f * constants::pi)) / (2.f * constants::pi);

	if (mOscillatorMode == OSCILLATOR_MODE_SINE) {
		value = naiveWaveformForMode(OSCILLATOR_MODE_SINE, phase.getValue());
	}
	else if (mOscillatorMode == OSCILLATOR_MODE_SAW) {
		value = naiveWaveformForMode(OSCILLATOR_MODE_SAW, phase.getValue());
		value -= poly_blep(t);
	}
	else {
		value = naiveWaveformForMode(OSCILLATOR_MODE_SQUARE, phase.getValue());
		value += poly_blep(t);
		float temp = BhMath::fmod(t + 0.5f, 1.f);
		value -= poly_blep(temp);
		if (mOscillatorMode == OSCILLATOR_MODE_TRIANGLE)
		{
			// Leaky integrator: y[n] = A * x[n] + (1 - A) * y[n-1]
			value = mPhaseIncrement * value + (1.f - mPhaseIncrement) * lastOutput;
			lastOutput = value;
		}
	}
	return value;
}
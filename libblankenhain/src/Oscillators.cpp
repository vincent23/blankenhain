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

void BaseOscillator::setFrequency(float frequency, float detune) 
{
	if (detune != 0.f)
	{
		unsigned int baseNote = aux::frequencyToNearestLowerNote(frequency);
		float freqLow = aux::noteToFrequency(baseNote == 0u ? 0u : baseNote - 1u);
		float freqHigh = aux::noteToFrequency(baseNote == 127u ? 127u : baseNote + 1u);
		if (detune > 0.f)
			frequency = (freqHigh - frequency) * detune + frequency;
		else
			frequency = (frequency - freqLow) * -1.f * detune + freqLow;
	}
	mFrequency = frequency;
    updateIncrement();
}

float NaiveOscillator::getSample(unsigned int time)
{
	mPhase.set(static_cast<float>(time) * mPhaseIncrement);
	float value = naiveWaveformForMode(mOscillatorMode);
	return value;
};

void BaseOscillator::updateIncrement() {
	//double calculatedFrequency = fmin(mFrequency, constants::sampleRate / 2.0);
	mPhaseIncrement = mFrequency * (2.f * constants::pi) / constants::sampleRate;
}

float NaiveOscillator::naiveWaveformForMode(NaiveOscillatorMode mode) {
	float value;
    switch (mode) {
        case OSCILLATOR_MODE_SINE:
            value = sin(mPhase.getValue());
            break;
        case OSCILLATOR_MODE_SAW:
            value = ( mPhase.getValue() / constants::pi) - 1.0;
            break;
        case OSCILLATOR_MODE_SQUARE:
            if (mPhase.getValue() < (constants::pi)) {
                value = 1.0;
            } else {
                value = -1.0;
            }
            break;
        case OSCILLATOR_MODE_TRIANGLE:
		{
			float tempvalue = -1.0 + (mPhase.getValue() / (constants::pi));
			value = 2.0 * (fabs(tempvalue) - 0.5);
			break;
		}
        default:
            break;
    }
    return value;
}

float NaiveOscillator::getNextSample()
{
	mPhase.incrementBy(mPhaseIncrement);
	float value = naiveWaveformForMode(mOscillatorMode);
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

float PolyBLEPOscillator::getSample(unsigned int time)
{
	float value = 0.0;

	mPhase.set(static_cast<float>(time) * mPhaseIncrement);

	float t = mPhase.getValue() / (2.f * constants::pi);

	if (mOscillatorMode == OSCILLATOR_MODE_SINE) {
		value = naiveWaveformForMode(OSCILLATOR_MODE_SINE);
	}
	else if (mOscillatorMode == OSCILLATOR_MODE_SAW) {
		value = naiveWaveformForMode(OSCILLATOR_MODE_SAW);
		value -= poly_blep(t);
	}
	else {
		value = naiveWaveformForMode(OSCILLATOR_MODE_SQUARE);
		value += poly_blep(t);
		float temp = fmod(t + 0.5f, 1.f);
		value -= poly_blep(temp);
		if (mOscillatorMode == OSCILLATOR_MODE_TRIANGLE) {
			// Leaky integrator: y[n] = A * x[n] + (1 - A) * y[n-1]
			value = mPhaseIncrement * value + (1. - mPhaseIncrement) * lastOutput;
			lastOutput = value;
		}
	}
	return value;
}
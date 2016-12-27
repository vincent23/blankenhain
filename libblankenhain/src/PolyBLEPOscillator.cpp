// via http://www.martin-finke.de/blog/articles/audio-plugins-018-polyblep-oscillator/
//
//  PolyBLEPOscillator.cpp
//  SpaceBass
//
//  Created by Martin on 08.04.14.
//
//

#include "PolyBLEPOscillator.h"


// PolyBLEP by Tale
// (slightly modified)
// http://www.kvraudio.com/forum/viewtopic.php?t=375517

float PolyBLEPOscillator::poly_blep(float& t)
{
    float dt = mPhaseIncrement / (2. * constants::pi);
    // 0 <= t < 1
    if (t < dt) 
	{
        t /= dt;
        return t+t - t*t - 1.0;
    }
    // -1 < t < 0
    else if (t > 1.0 - dt)
	{
        t = (t - 1.0) / dt;
        return t*t + t+t + 1.0;
    }
    // 0 otherwise
	else
	{
		return 0.0;
	}
}


float PolyBLEPOscillator::nextSample()
{
	float value = 0.0;
    float t = mPhase / (2.f * constants::pi);
    
    if (mOscillatorMode == OSCILLATOR_MODE_SINE) {
        value = naiveWaveformForMode(OSCILLATOR_MODE_SINE);
    } else if (mOscillatorMode == OSCILLATOR_MODE_SAW) {
        value = naiveWaveformForMode(OSCILLATOR_MODE_SAW);
        value -= poly_blep(t);
    } else {
        value = naiveWaveformForMode(OSCILLATOR_MODE_SQUARE);
        value += poly_blep(t);
		float temp = fmod(t + 0.f, 1.f);
        value -= poly_blep(temp);
        if (mOscillatorMode == OSCILLATOR_MODE_TRIANGLE) {
            // Leaky integrator: y[n] = A * x[n] + (1 - A) * y[n-1]
            value = ((mPhaseIncrement) * value + (1. - mPhaseIncrement)) * lastOutput;
            lastOutput = value;
        }
    }
    
    mPhase += mPhaseIncrement;
    while (mPhase >= (2. * constants::pi)) {
        mPhase -= (2. * constants::pi);
    }
    return value;
}

float PolyBLEPOscillator::getSample(unsigned int time)
{
	float value = 0.0;

	mPhase = static_cast<float>(time) * mPhaseIncrement;
	while (mPhase >= (2. * constants::pi)) {
		mPhase -= (2. * constants::pi);
	}

	float t = mPhase / (2.f * constants::pi);

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
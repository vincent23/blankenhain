//
// via http://www.martin-finke.de/blog/articles/audio-plugins-018-polyblep-oscillator/
//  Oscillator.cpp
//  SpaceBass
//
//  Created by Martin on 08.04.14.
//
//

#include "Oscillator.h"


Oscillator::Oscillator() :
	mOscillatorMode(OSCILLATOR_MODE_SINE),
	mFrequency(440.0),
	mPitchMod(0.0),
	mPhase(0.0) 
{
	updateIncrement();
};


void Oscillator::setMode(OscillatorMode mode) {
    mOscillatorMode = mode;
}

void Oscillator::setFrequency(double frequency) {
    mFrequency = frequency;
    updateIncrement();
}

/*
void Oscillator::generate(double* buffer, int nFrames) {
    const double twoPI = 2 * constants::pi;
    switch (mOscillatorMode) {
        case OSCILLATOR_MODE_SINE:
            for (int i = 0; i < nFrames; i++) {
                buffer[i] = sin(mPhase);
                mPhase += mPhaseIncrement;
                while (mPhase >= (2.f * constants::pi)) {
                    mPhase -= (2.f * constants::pi);
                }
            }
            break;
        case OSCILLATOR_MODE_SAW:
            for (int i = 0; i < nFrames; i++) {
                buffer[i] = 1.0 - (2.0 * mPhase / (2.f * constants::pi));
                mPhase += mPhaseIncrement;
                while (mPhase >= (2.f * constants::pi)) {
                    mPhase -= (2.f * constants::pi);
                }
            }
            break;
        case OSCILLATOR_MODE_SQUARE:
            for (int i = 0; i < nFrames; i++) {
                if (mPhase <= constants::pi) {
                    buffer[i] = 1.0;
                } else {
                    buffer[i] = -1.0;
                }
                mPhase += mPhaseIncrement;
                while (mPhase >= twoPI) {
                    mPhase -= twoPI;
                }
            }
            break;
        case OSCILLATOR_MODE_TRIANGLE:
            for (int i = 0; i < nFrames; i++) {
                double value = -1.0 + (2.0 * mPhase / twoPI);
                buffer[i] = 2.0 * (fabs(value) - 0.5);
                mPhase += mPhaseIncrement;
                while (mPhase >= twoPI) {
                    mPhase -= twoPI;
                }
            }
            break;
    }
}
*/

float Oscillator::nextSample()
{
	float value = naiveWaveformForMode(mOscillatorMode);
    mPhase += mPhaseIncrement;
    while (mPhase >= (2.f * constants::pi)) {
        mPhase -= (2.f * constants::pi);
    }
    return value;
}

void Oscillator::setPitchMod(double amount) {
    mPitchMod = amount;
    updateIncrement();
}

void Oscillator::updateIncrement() {
    double pitchModAsFrequency = pow(2.0, fabs(mPitchMod) * 14.0) - 1;
    if (mPitchMod < 0) {
        pitchModAsFrequency = -pitchModAsFrequency;
    }
    double calculatedFrequency = fmin(fmax(mFrequency + pitchModAsFrequency, 0), constants::sampleRate /2.0);
    mPhaseIncrement = calculatedFrequency * (2.f * constants::pi) / constants::sampleRate;
}

float Oscillator::naiveWaveformForMode(OscillatorMode mode) {
	float value;
    switch (mode) {
        case OSCILLATOR_MODE_SINE:
            value = sin(mPhase);
            break;
        case OSCILLATOR_MODE_SAW:
            value = ( mPhase / constants::pi) - 1.0;
            break;
        case OSCILLATOR_MODE_SQUARE:
            if (mPhase < (constants::pi)) {
                value = 1.0;
            } else {
                value = -1.0;
            }
            break;
        case OSCILLATOR_MODE_TRIANGLE:
		{
			float tempvalue = -1.0 + (mPhase / (constants::pi));
			value = 2.0 * (fabs(tempvalue) - 0.5);
			break;
		}
        default:
            break;
    }
    return value;
}

// via http://www.martin-finke.de/blog/articles/audio-plugins-018-polyblep-oscillator/
//
//  Oscillator.h
//  SpaceBass
//
//  Created by Martin on 08.04.14.
//
//

#pragma once
#include <math.h>
#include "Constants.h"
#include "Sample.h"

class Oscillator {
public:

	Oscillator();

    enum OscillatorMode 
	{
        OSCILLATOR_MODE_SINE = 0,
        OSCILLATOR_MODE_SAW,
        OSCILLATOR_MODE_SQUARE,
        OSCILLATOR_MODE_TRIANGLE,
        kNumOscillatorModes
    };

    void setMode(OscillatorMode mode);
    void setFrequency(double frequency);

    //void generate(double* buffer, int nFrames);

    virtual float nextSample();
    void reset() 
	{ 
		mPhase = 0.0; 
	}

    void setPitchMod(double amount);
protected:
    float naiveWaveformForMode(OscillatorMode mode);
	void updateIncrement();

    OscillatorMode mOscillatorMode;
    float mFrequency;
    float mPhase;
    float mPhaseIncrement;
    float mPitchMod;
};

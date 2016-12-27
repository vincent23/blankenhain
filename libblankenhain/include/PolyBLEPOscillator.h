// via http://www.martin-finke.de/blog/articles/audio-plugins-018-polyblep-oscillator/
//
//  PolyBLEPOscillator.h
//  SpaceBass
//
//  Created by Martin on 08.04.14.
//
//

#pragma once
#include "Oscillator.h"

class PolyBLEPOscillator : public Oscillator {
public:
    PolyBLEPOscillator() : lastOutput(0.0) 
	{ 
		updateIncrement(); 
	};
	float nextSample() override;
	float getSample(unsigned int time);
private:
    float poly_blep(float& t);
    float lastOutput;
};

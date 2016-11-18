/*
  ==============================================================================

    AuxFunc.h
    Created: 1 Apr 2016 1:37:48pm
    Author:  Dustin Kaiser

  ==============================================================================
*/

#include <math.h>
#pragma once

namespace aux
{
  float linearToDecibel(float linear);

  float decibelToLinear(float db);

  float millisecToSamples(float millisec, float samplerate = 44100.f);

  float samplesToMillisec(float samples, float samplerate = 44100.f);
}


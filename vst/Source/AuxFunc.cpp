/*
  ==============================================================================

    AuxFunc.cpp
    Created: 1 Apr 2016 1:37:48pm
    Author:  Dustin Kaiser

  ==============================================================================
*/

#include "AuxFunc.h"

// Auxiliary helper functions
namespace aux
{
  float linearToDecibel(float linear)
  {
    float db;

    if (linear != 0.0f)
      db = 20.0f * log10(linear);
    else
      db = -144.0f;  // effectively minus infinity

    return db;
  }

  float decibelToLinear(float db)
  {
    float linear;

    if (db < -144.0f)
      linear = 0.f;
    else
      linear = pow(10.f, db / 20.f);

    return linear;
  }
}
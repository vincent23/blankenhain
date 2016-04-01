/*
  ==============================================================================

    AuxFunc.h
    Created: 1 Apr 2016 1:37:48pm
    Author:  Dustin Kaiser

  ==============================================================================
*/

#ifndef AUXFUNC_H_INCLUDED
#define AUXFUNC_H_INCLUDED
#include <math.h>
#pragma once

namespace aux
{
  float linearToDecibel(float linear);

  float decibelToLinear(float db);
}




#endif  // AUXFUNC_H_INCLUDED

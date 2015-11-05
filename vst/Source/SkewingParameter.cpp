/*
  ==============================================================================

    SkewingParameter.cpp
    Created: 5 Nov 2015 8:56:45am
    Author:  Dusti

  ==============================================================================
*/

#include "SkewingParameter.h"

SkewingParameter::SkewingParameter()
			     : skewFactorLeft(1.f),
			     skewFactorRight(1.f),
			     skewPoint(0.5f) {};
                 
SkewingParameter::SkewingParameter(float skewFactorLeft_, float skewPoint_, float skewFactorRight_)
			     : skewFactorLeft(skewFactorLeft_),
			     skewFactorRight(skewFactorRight_),
			     skewPoint(skewPoint_) {};

SkewingParameter::SkewingParameter(float skewFactorLeft_, float skewPointUNNORMALIZED, float skewFactorRight_, \
							    	float beginRange, float endRange)
			 	: skewFactorLeft(skewFactorLeft_),
			 	skewFactorRight(skewFactorRight_),
			 	skewPoint(skewPointUNNORMALIZED / (endRange - beginRange)) {};

float SkewingParameter::getSkewedValueFromNormalized(float normalizedUnscewed) const
{
    //DUMMY
	return 1.f;
}

float SkewingParameter::getSkewedValueFromRange(float unnormalizedUnscewed, float beginRange, \
                     float endRange) const
{
    //DUMMY
	return 1.f;
}
                     
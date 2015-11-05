/*
  ==============================================================================

    SkewingParameter.h
    Created: 5 Nov 2015 8:56:45am
    Author:  Dusti

  ==============================================================================
*/

#ifndef SKEWINGPARAMETER_H_INCLUDED
#define SKEWINGPARAMETER_H_INCLUDED

//Class to handle skewing
//This class ALWAYS WORKS ON NORMALIZED FLOATS
//so teach your classes getters and setters how
//to freaking normalize, jesus christ dude.
//Oh, and yes, the input values also need to
//be between fuckin 0.f and 1.f, its not
//that hard dude. shit.


class SkewingParameter
{
public:
SkewingParameter(); //default constructor, no skewing
SkewingParameter(float skewFactorLeft_, float skewPoint_, float skedFactorRight_);

float getSkewedValueFromNormalized(float normalizedUnskewed) const;

//Ok since you can't keep your shit together
//here is the freaking method for frenchmen like
//yourself
float getSkewedValueFromRange(float unnormalizedUnscewed, float beginRange = 0.f, \
                     float endRange = 1.f) const;
//You happy now?

SkewingParameter(float skewFactorLeft_, float skewPointUNNORMALIZED, float skedFactorRight_, \
                 float beginRange, float endRange);
//...motherf*ck
                     
private:
float skewFactorLeft;
float skewFactorRight;
float skewPoint;
};


#endif  // SKEWINGPARAMETER_H_INCLUDED

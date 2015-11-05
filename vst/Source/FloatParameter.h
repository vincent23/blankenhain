#ifndef FLOAT_PARAMETER_H_INCLUDED
#define FLOAT_PARAMETER_H_INCLUDED

#include <juce>
#include <functional>
#include "SkewingParameter.h"

// Internally all floats should be normalized, since 
// this is necessary for communication with DAW-Host.
// Therefore, you need to specify to lambdas for conversion
// to and from normalized values for your stuff

class FloatParameter : public AudioProcessorParameter
{
public:
	//USE THIS CONSTRUCTOR
	FloatParameter(float defaultValue, const String& name = "I was not initialized properly. U bad boy mhmhm.", \
		float bufferScalingValue_ = 1.f, SkewingParameter skewingValue_ = SkewingParameter(), \
		std::function<float(float)> conversionFromNormalized = [](float in) {return in; }, \
		std::function<float(float)> conversionToNormalized = [](float in) \
		{return in; });
	//COPY BY REFERENCE
	FloatParameter(FloatParameter const&);
	//LEGACY CONSTRCUTOR DONT USE THIS
	FloatParameter(const String& name, float defaultValue, \
		float bufferScalingValue_ = 1.f, SkewingParameter skewingValue_ = SkewingParameter(), \
		std::function<float(float)> conversionFromNormalized = [](float in) {return in; }, \
		std::function<float(float)> conversionToNormalized = [](float in) \
		{return in; });


	float getValue() const override;
	float getOldValue() const;
	void setOldValue(float oldValue_);
	void setValue(float newValue) override;
	float getDefaultValue() const override;
	float getBufferScalingValue() const;
	String getName(int maximumStringLength) const override;
	String getLabel() const override;
	float getValueForText(const String &text) const override;
	std::function<float(float)> getConversionFromNormalized() const;
	std::function<float(float)> getConversionToNormalized() const;
	float getNormalizedValue() const;

private:
	const float defaultValue;
	const SkewingParameter skewingValue;
	float value;
	float oldValue;
	const float bufferScalingValue;
	// This Value should be between 0 and 1 and specifies the end of
	// linear interpolation in terms of "percentage of whole buffer"
	
	const String name; 
	const std::function<float(float)> conversionToNormalized; 
	//This Lambda should be specified uppon construction
	//takes float as input and returns float
	//and should handle the conversion from 
	//		UNNORMALIZED
	//			TO
	//		NORMALIZED

	const std::function<float(float)> conversionFromNormalized;
	//Similarly, this will perform the opposite
	//conversion and also needs to be specified
};

#endif // FLOAT_PARAMETER_H_INCLUDED
#pragma once

#include <juce>
#include <functional>

// Internally all floats should be normalized, since 
// this is necessary for communication with DAW-Host.
// Therefore, you need to specify to lambdas for conversion
// to and from normalized values for your stuff

class FloatParameter : public AudioProcessorParameter
{
public:
	//USE THIS CONSTRUCTOR
	FloatParameter(float defaultValue, const String& name = "I was not initialized properly. U bad boy mhmhm.", \
		float bufferScalingValue_ = 1.f, const NormalizedRange& normalizedRange = NormalizedRange());
	//COPY BY REFERENCE
	FloatParameter(const FloatParameter&);

	float getValue() const override;
	void setValue(float unnormalizedNewValue_) override;

	float getOldValue() const;
	void setOldValue(float unnormalizedOldValue_);
	void setOldValue(void);

	float getDefaultValue() const override;
	float getBufferScalingValue() const;

	String getName(int maximumStringLength) const override;
	String getLabel() const override;
	float getValueForText(const String &text) const override;

	float getNormalizedValue() const;
	float getNormalizedOldValue() const;
	float getNormalizedDefaultValue() const;

private:
	const float defaultValue;
	NormalizedRange range;
	float value;
	float oldValue;
	const float bufferScalingValue;
	// This Value should be between 0 and 1 and specifies the end of
	// linear interpolation in terms of "percentage of whole buffer"
	
	const String name; 
};
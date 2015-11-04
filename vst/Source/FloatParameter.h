#ifndef FLOAT_PARAMETER_H_INCLUDED
#define FLOAT_PARAMETER_H_INCLUDED

#include <juce>

class FloatParameter : public AudioProcessorParameter
{
public:
	FloatParameter(const String& name, float defaultValue, float scalingValue = 1.f);

	float getValue() const override;
	float getOldValue() const;
	void setOldValue(float oldValue_);
	void setValue(float newValue) override;
	float getDefaultValue() const override;
	float getScalingValue() const;
	String getName(int maximumStringLength) const override;
	String getLabel() const override;
	float getValueForText(const String &text) const override;

private:
	const float defaultValue;
	float value;
	float oldValue;
	const float scalingValue;	// This Value should be between 0 and 1 and specifies the end of
						// linear interpolation in terms of "percentage of whole buffer"
	const String name;
};

#endif // FLOAT_PARAMETER_H_INCLUDED
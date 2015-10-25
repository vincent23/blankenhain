#ifndef FLOAT_PARAMETER_H_INCLUDED
#define FLOAT_PARAMETER_H_INCLUDED

#include <juce>

class FloatParameter : public AudioProcessorParameter
{
public:
	FloatParameter(const String& name, float defaultValue);

	float getValue() const override;
	void setValue(float newValue) override;
	float getDefaultValue() const override;
	String getName(int maximumStringLength) const override;
	String getLabel() const override;
	float getValueForText(const String &text) const override;

private:
	const float defaultValue;
	float value;
	const String name;
};

#endif // FLOAT_PARAMETER_H_INCLUDED
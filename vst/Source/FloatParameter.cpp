#include "FloatParameter.h"

FloatParameter::FloatParameter(float defaultValue_, const String& name_, float scalingValue_, NormalisableRange<float> normalisableRange_)
			   :defaultValue(normalisableRange_.convertTo0to1(defaultValue_)), value(normalisableRange_.convertTo0to1(defaultValue_)), \
			    oldValue(normalisableRange_.convertTo0to1(defaultValue_)), name(name_), range(normalisableRange_), \
			    bufferScalingValue(scalingValue_)
{
};

FloatParameter::FloatParameter(const String& name_, float defaultValue_, float scalingValue_, NormalisableRange<float> normalisableRange_)
			   : defaultValue(normalisableRange_.convertTo0to1(defaultValue_)), value(normalisableRange_.convertTo0to1(defaultValue_)), \
			   oldValue(normalisableRange_.convertTo0to1(defaultValue_)), name(name_), \
			   bufferScalingValue(scalingValue_)
{
};

FloatParameter::FloatParameter(FloatParameter const& in)
		       :defaultValue(in.defaultValue), value(in.value), \
		       oldValue(in.oldValue), name(in.name), \
		       bufferScalingValue(in.bufferScalingValue), range(in.range) {};

float FloatParameter::getOldValue() const {
	return range.convertFrom0to1(oldValue);
}

float FloatParameter::getBufferScalingValue() const {
	return bufferScalingValue;
}

void FloatParameter::setOldValue(float oldValue_) {
	oldValue = range.convertTo0to1(oldValue_);
}

float FloatParameter::getValue() const {
	return range.convertFrom0to1(value);
}

void FloatParameter::setValue(float newValue_) {
	value = range.convertTo0to1(newValue_);
}

float FloatParameter::getDefaultValue() const {
	return range.convertFrom0to1(defaultValue);
}

String FloatParameter::getName(int maximumStringLength) const {
	if (name.length() <= maximumStringLength) {
		return name;
	}
	else {
		return name.dropLastCharacters(name.length() - maximumStringLength);
	}
}

String FloatParameter::getLabel() const {
	return String();
}

float FloatParameter::getValueForText(const String &text) const {
	return text.getFloatValue();
}

float FloatParameter::getNormalizedValue() const
{
	return value;
}
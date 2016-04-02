#include "FloatParameter.h"

FloatParameter::FloatParameter(float defaultValue_, const String& name_, float scalingValue_, const NormalizedRange& normalizedRange_)
	:defaultValue(normalizedRange_.toNormalized(defaultValue_)), value(normalizedRange_.toNormalized(defaultValue_)), \
	oldValue(normalizedRange_.toNormalized(defaultValue_)), name(name_), range(normalizedRange_), \
	bufferScalingValue(scalingValue_)
{
};

FloatParameter::FloatParameter(const FloatParameter& in)
	:defaultValue(in.defaultValue), value(in.value), \
	oldValue(in.oldValue), name(in.name), \
	bufferScalingValue(in.bufferScalingValue), range(in.range) {};

float FloatParameter::getUnnormalizedOldValue() const {
	return range.fromNormalized(oldValue);
}

float FloatParameter::getBufferScalingValue() const {
	return bufferScalingValue;
}

float FloatParameter::getUnnormalizedValue() const
{
  return range.fromNormalized(value);
}
void FloatParameter::setUnnormalizedValue(float unnormalized)
{
 this->setValueNotifyingHost(range.toNormalized(unnormalized));
}

void FloatParameter::setUnnormalizedOldValue(float oldValue_) {
	oldValue = range.toNormalized(oldValue_);
}

void FloatParameter::setUnnormalizedOldValue(void) {
	oldValue = value;
}

// Note: Needs to return normalized values
float FloatParameter::getValue() const 
{
	return value;
}

// Note: Expects normalized values
void FloatParameter::setValue(float newValue_) {
	value = newValue_;
}

// Returns value between 0 and 1
float FloatParameter::getDefaultValue() const {
	return defaultValue;
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

float FloatParameter::getNormalizedOldValue() const
{
	return oldValue;
}

float FloatParameter::getNormalizedDefaultValue() const
{
	return defaultValue;
}
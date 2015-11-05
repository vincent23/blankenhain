#include "FloatParameter.h"

FloatParameter::FloatParameter(float defaultValue_, const String& name_, float scalingValue_, SkewingParameter skewingValue_,\
							   std::function<float(float)> conversionFromNormalized_, \
							   std::function<float(float)> conversionToNormalized_)
			   :defaultValue(conversionToNormalized_(defaultValue_)), value(conversionToNormalized_(defaultValue_)), \
			    oldValue(conversionToNormalized_(defaultValue_)), name(name_), \
			    bufferScalingValue(scalingValue_), skewingValue(skewingValue_), \
				conversionFromNormalized(conversionFromNormalized_), \
				conversionToNormalized(conversionToNormalized_)
{
};

FloatParameter::FloatParameter(const String& name_, float defaultValue_, float scalingValue_, SkewingParameter skewingValue_, \
							   std::function<float(float)> conversionFromNormalized_, \
							   std::function<float(float)> conversionToNormalized_)
			   : defaultValue(conversionToNormalized_(defaultValue_)), value(conversionToNormalized_(defaultValue_)), \
			   oldValue(conversionToNormalized_(defaultValue_)), name(name_), \
			   bufferScalingValue(scalingValue_), skewingValue(skewingValue_), \
			   conversionFromNormalized(conversionFromNormalized_), \
			   conversionToNormalized(conversionToNormalized_)
{
};

FloatParameter::FloatParameter(FloatParameter const& in)
		       :defaultValue(in.defaultValue), value(in.value), \
		       oldValue(in.oldValue), name(in.name), \
		       bufferScalingValue(in.bufferScalingValue), skewingValue(in.skewingValue), \
		       conversionFromNormalized(in.conversionFromNormalized), \
		       conversionToNormalized(in.conversionToNormalized) {};

float FloatParameter::getOldValue() const {
	return conversionFromNormalized(oldValue);
}

float FloatParameter::getBufferScalingValue() const {
	return bufferScalingValue;
}

void FloatParameter::setOldValue(float oldValue_) {
	oldValue = conversionToNormalized(oldValue_);
}

float FloatParameter::getValue() const {
	return conversionFromNormalized(value);
}

void FloatParameter::setValue(float newValue_) {
	value = conversionToNormalized(newValue_);
}

float FloatParameter::getDefaultValue() const {
	return conversionFromNormalized(defaultValue);
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

std::function<float(float)> FloatParameter::getConversionFromNormalized() const
{
	return this->conversionFromNormalized;
}

std::function<float(float)> FloatParameter::getConversionToNormalized() const
{
	return this->conversionToNormalized;
}

float FloatParameter::getNormalizedValue() const
{
	return value;
}
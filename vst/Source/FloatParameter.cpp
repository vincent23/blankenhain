#include "FloatParameter.h"

FloatParameter::FloatParameter(const String& name_, float defaultValue_)
	: defaultValue(defaultValue_), value(defaultValue_), name(name_)
{
}

float FloatParameter::getValue() const {
	return value;
}

void FloatParameter::setValue(float newValue) {
	value = newValue;
}

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

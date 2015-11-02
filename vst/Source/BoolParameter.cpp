#include "BoolParameter.h"

BoolParameter::BoolParameter(const String& name_, bool defaultValue_)
	: defaultValue(defaultValue_), value(defaultValue_), name(name_)
{
}

float BoolParameter::getValue() const {
	return value;
}

int BoolParameter::getNumSteps() const {
	return 2;
}

void BoolParameter::setValue(float newValue) {
	if (newValue >= 0.5f)
	{
		value = true;
	}
	else
	{
		value = false;
	}
}

float BoolParameter::getDefaultValue() const {
	return defaultValue;
}

bool BoolParameter::getBoolValue() const {
	return value;
}

void BoolParameter::setBoolValue(bool newValue) {
	value = newValue;
}

String BoolParameter::getName(int maximumStringLength) const {
	if (name.length() <= maximumStringLength) {
		return name;
	}
	else {
		return name.dropLastCharacters(name.length() - maximumStringLength);
	}
}

String BoolParameter::getLabel() const {
	return String();
}

float BoolParameter::getValueForText(const String &text) const {
	return text.getFloatValue();
}

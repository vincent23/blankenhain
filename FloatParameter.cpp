#include "FloatParameter.h"

FloatParameter::FloatParameter()
  :
  oldValue(0.f), currentValue(0.f)
{
};


float ParameterWithProperties::getOldValueUnnormalized() const {
	return fromNormalized(oldValue);
}

void FloatParameter::setOldValueNormalized(float const& in)
{
  oldValue = in;
}

void FloatParameter::setCurrentValueNormalized(float const& in)
{
  currentValue = in;
}

ParameterWithProperties::ParameterWithProperties(float defaultValueUnnormalized, 
  NormalizedRange const& range,
   String name_, String unit_) :
  NormalizedRange(range), FloatParameter(), 
  normalizedDefaultValue(range.toNormalized(defaultValueUnnormalized)),
  name(name_), unit(unit_)
{
  this->setToDefaultValue();
}

float ParameterWithProperties::getCurrentValueUnnormalized() const
{
  return fromNormalized(currentValue);
}

void ParameterWithProperties::setCurrentValueUnnormalized(float unnormalized)
{
  currentValue = toNormalized(unnormalized);
}

void ParameterWithProperties::setOldValueUnnormalized(float oldValue_) {
	oldValue = toNormalized(oldValue_);
}

// Returns value between 0 and 1
float ParameterWithProperties::getDefaultValueUnnormalized() const {
	return fromNormalized(normalizedDefaultValue);
}

std::string ParameterWithProperties::getName(unsigned int maximumStringLength) const 
{
  if (maximumStringLength == 0u || name.length() <= maximumStringLength) return this->name;
	else 
  {
    return name.substr(0u, maximumStringLength);
	}
}

std::string ParameterWithProperties::getUnit() const
{
  return this->unit;
}

float FloatParameter::getCurrentValueNormalized() const
{
	return currentValue;
}

float FloatParameter::getOldValueNormalized() const
{
	return oldValue;
}

void ParameterWithProperties::setToDefaultValue()
{
  currentValue = normalizedDefaultValue;
}

float ParameterWithProperties::getDefaultValueNormalized() const
{
	return normalizedDefaultValue;
}
#include "FloatParameter.h"

FloatParameter::FloatParameter()
  :
  oldValueNormalized(0.f), targetValueNormalized(0.f)
{
};

float ParameterWithProperties::getOldValueUnnormalized() const {
	return fromNormalized(oldValueNormalized);
}

void FloatParameter::setOldValueNormalized(float const& in)
{
  oldValueNormalized = in;
}

float FloatParameter::getInterpolatedNormmalized(size_t currentStep, size_t maxSteps)
{
  return (oldValueNormalized + (float(maxSteps - currentStep + 1) * (targetValueNormalized - oldValueNormalized) \
    / float(maxSteps)));
}

float ParameterWithProperties::getInterpolatedUnnormmalized(size_t currentStep, size_t maxSteps) const
{
  return (this->getOldValueUnnormalized() + (float(maxSteps - currentStep + 1) * (this->getTargetValueUnnormalized() - this->getOldValueUnnormalized()) \
    / float(maxSteps)));
}

void FloatParameter::setTargetValueNormalized(float const& in)
{
  targetValueNormalized = in;
}

ParameterWithProperties::ParameterWithProperties(float defaultValueUnnormalized, 
  NormalizedRange const& range,
   String name_, String unit_) :
  NormalizedRange(range), FloatParameter(), 
  normalizedDefaultValue(range.toNormalized(defaultValueUnnormalized)),
  name(name_), unit(unit_), interpolationIteration(0u), immediateValueNormalized(toNormalized(defaultValueUnnormalized))
{
  this->setToDefaultValue();
}

float ParameterWithProperties::getTargetValueUnnormalized() const
{
  return fromNormalized(targetValueNormalized);
}

void ParameterWithProperties::setTargetValueUnnormalized(float unnormalized)
{
  targetValueNormalized = toNormalized(unnormalized);
}

void ParameterWithProperties::setOldValueUnnormalized(float oldValue_) {
	oldValueNormalized = toNormalized(oldValue_);
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

float FloatParameter::getTargetValueNormalized() const
{
	return targetValueNormalized;
}

float FloatParameter::getOldValueNormalized() const
{
	return oldValueNormalized;
}

void ParameterWithProperties::setToDefaultValue()
{
  targetValueNormalized = normalizedDefaultValue;
}

float ParameterWithProperties::getDefaultValueNormalized() const
{
	return normalizedDefaultValue;
}

float ParameterWithProperties::getImmediateValueAndUpdateUnnormalized()
{
  return fromNormalized(this->incrementImmediateValueAndReturnOldValueNormalized());
}

float ParameterWithProperties::getImmediateValueUnnormalized()
{
  return fromNormalized(this->immediateValueNormalized);
}

bool ParameterWithProperties::oldAndTargetValueMatch() const
{
  return (oldValueNormalized == targetValueNormalized);
}

float ParameterWithProperties::getImmediateValueNormalized()
{
  return this->immediateValueNormalized;
}


float ParameterWithProperties::incrementImmediateValueAndReturnOldValueNormalized()
{
  float old = this->immediateValueNormalized;
  immediateValueNormalized = oldValueNormalized + ((oldValueNormalized - targetValueNormalized) / (interpolationMax) ) * interpolationIteration;
  return old;
}

float ParameterWithProperties::getImmediateValueAndUpdateNormalized()
{
  return this->incrementImmediateValueAndReturnOldValueNormalized();
}
void ParameterWithProperties::setImmediateValueNormalized(float in)
{
  this->immediateValueNormalized = in;
}
void ParameterWithProperties::setImmediateValueUnnormalized(float in)
{
  this->immediateValueNormalized = toNormalized(in);
}
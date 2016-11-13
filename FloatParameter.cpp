#include "FloatParameter.h"

FloatParameter::FloatParameter(float defaultNormalized)
  :
  oldValueNormalized(defaultNormalized), targetValueNormalized(defaultNormalized), normalizedDefaultValue(defaultNormalized)
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
  NormalizedRange(range), FloatParameter(range.toNormalized(defaultValueUnnormalized)),
  name(name_), unit(unit_), interpolationIteration(0u), immediateValueNormalized(normalizedDefaultValue),
  interpolationDistance(0u)
{
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

//void FloatParameter::setToDefaultValue()
//{
//  targetValueNormalized = normalizedDefaultValue;
//}

float FloatParameter::getDefaultValueNormalized() const
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
  interpolationIteration++;
  immediateValueNormalized = oldValueNormalized + ((targetValueNormalized - oldValueNormalized) / (interpolationDistance) ) * interpolationIteration;
  return old;
}

void ParameterWithProperties::setInterpolationDistance(size_t const& dist)
{
  this->interpolationDistance = dist;
}
void ParameterWithProperties::resetInterpolationCounter()
{
  this->interpolationIteration = 0u;
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
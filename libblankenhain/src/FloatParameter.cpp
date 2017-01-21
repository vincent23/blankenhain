#include "FloatParameter.h"

#include "Constants.h"

FloatParameter::FloatParameter(float defaultValueUnnormalized, const NormalizedRange& range, BhString name_, BhString unit_)
	: NormalizedRange(range)
	, defaultValueNormalized(range.toNormalized(defaultValueUnnormalized))
	, valueNormalized(range.toNormalized(defaultValueUnnormalized))
	, name(name_)
	, unit(unit_)
{ 
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	if (! range.isInRange(defaultValueUnnormalized))
		throw std::runtime_error("default value not in range\n");
#endif
}

float FloatParameter::getDefaultValueNormalized() const {
	return defaultValueNormalized;
}

void FloatParameter::setToDefaultValue()
{
	unsigned int interpolationLength = constants::parameterInterpolationLength;
	valueNormalized = InterpolatedValue<float>(this->getValueNormalized(), this->defaultValueNormalized, interpolationLength);
}


float FloatParameter::getValueNormalized() const
{
	return valueNormalized.get();
}

float FloatParameter::getValueNormalized()
{
	return valueNormalized.get();
}

float FloatParameter::getValueUnnormalized() const
{
	return fromNormalized(getValueNormalized());
}

float FloatParameter::getValueUnnormalized()
{
	return fromNormalized(getValueNormalized());
}

BhString FloatParameter::getName(unsigned int maximumStringLength) const
{
	if (maximumStringLength == 0u || name.length() <= maximumStringLength) {
		return name;
	}
	else {
		return name.substr(0u, maximumStringLength);
	}
}

BhString FloatParameter::getUnit() const
{
	return unit;
}

void FloatParameter::setTargetValueNormalized(float normalizedValue)
{
	unsigned int interpolationLength = constants::parameterInterpolationLength;
	valueNormalized = InterpolatedValue<float>(this->getValueNormalized(), normalizedValue, interpolationLength);
}

void FloatParameter::setTargetValueUnnormalized(float unnormalizedValue)
{
	setTargetValueNormalized(toNormalized(unnormalizedValue));
}

void FloatParameter::next(unsigned int numberOfSamples)
{
	valueNormalized.next(numberOfSamples);
}
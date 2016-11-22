#include "FloatParameter.h"

#include "Constants.h"

FloatParameter::FloatParameter(float defaultValueUnnormalized, const NormalizedRange& range, String name_, String unit_)
	: NormalizedRange(range)
	, defaultValueNormalized(range.toNormalized(defaultValueUnnormalized))
	, currentValueNormalized(defaultValueNormalized)
	, name(name_)
	, unit(unit_)
{ }

float FloatParameter::getDefaultValueNormalized() const {
	return defaultValueNormalized;
}

float FloatParameter::getValueNormalized() const
{
	return currentValueNormalized;
}

float FloatParameter::getValueUnnormalized() const
{
	return fromNormalized(getValueNormalized());
}

String FloatParameter::getName(unsigned int maximumStringLength) const
{
	if (maximumStringLength == 0u || name.length() <= maximumStringLength) {
		return name;
	}
	else {
		return name.substr(0u, maximumStringLength);
	}
}

String FloatParameter::getUnit() const
{
	return unit;
}

void FloatParameter::setTargetValueNormalized(float normalizedValue)
{
	interpolationCounter = constants::parameterInterpolationLength;
	interpolationStepSize = (normalizedValue - currentValueNormalized) / interpolationCounter;
}

void FloatParameter::setTargetValueUnnormalized(float unnormalizedValue)
{
	setTargetValueNormalized(toNormalized(unnormalizedValue));
}

void FloatParameter::next(unsigned int numberOfSamples)
{
	if (interpolationCounter < numberOfSamples) {
		numberOfSamples = interpolationCounter;
	}
	currentValueNormalized += numberOfSamples * interpolationStepSize;
	interpolationCounter -= numberOfSamples;
}
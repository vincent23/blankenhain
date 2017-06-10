#include "FloatParameter.h"

#include "Constants.h"

FloatParameter::FloatParameter(float defaultValueUnnormalized, const NormalizedRange& range, BhString name_, BhString unit_)
	: NormalizedRange(range)
	, defaultValueNormalized(range.toNormalized(defaultValueUnnormalized))
	, valueNormalized(range.toNormalized(defaultValueUnnormalized))
#ifndef _VC_NODEFAULTLIB
	, name(name_)
	, unit(unit_)
#endif
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
	valueNormalized = BoundrySafeInterpolatedValue<float>(this->getValueNormalized(), this->defaultValueNormalized, interpolationLength);
}

const bool FloatParameter::canBeModulated() const
{
	return true;
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
#ifndef _VC_NODEFAULTLIB
	if (maximumStringLength == 0u || name.length() <= maximumStringLength) {
		return name;
	}
	else {
		return name.substr(0u, maximumStringLength);
	}
#else
	return nullptr;
#endif
}

BhString FloatParameter::getUnit() const
{
#ifndef _VC_NODEFAULTLIB
	return unit;
#else
	return nullptr;
#endif
}

void FloatParameter::setTargetValueNormalized(float normalizedValue)
{
	constexpr unsigned int interpolationLength = constants::parameterInterpolationLength;
	valueNormalized = BoundrySafeInterpolatedValue<float>(this->getValueNormalized(), normalizedValue, interpolationLength);
}

void FloatParameter::setTargetValueUnnormalized(float unnormalizedValue)
{
	setTargetValueNormalized(toNormalized(unnormalizedValue));
}

void FloatParameter::setCurrentValueNormalized(float normalizedValue)
{
	valueNormalized = BoundrySafeInterpolatedValue<float>(normalizedValue);
}

void FloatParameter::next(unsigned int numberOfSamples)
{
	valueNormalized.next(numberOfSamples);
}

BoolParameter::BoolParameter(bool defaultValue, BhString name)
	: FloatParameter(static_cast<float>(defaultValue), NormalizedRange(), name, "") {};

void BoolParameter::setTargetValueNormalized(float normalizedValue)
{
	valueNormalized = BoundrySafeInterpolatedValue<float>(normalizedValue);
}

void BoolParameter::next(unsigned int numberOfSamples)
{
	// no interpolation for IntegerParam
}

void BoolParameter::setTargetValueUnnormalized(float unnormalizedValue) 
{
	this->setTargetValueNormalized(unnormalizedValue);
}

bool BoolParameter::getValue() const
{
	return this->valueNormalized.get() == 1.f;
}

void BoolParameter::flip()
{
	valueNormalized = BoundrySafeInterpolatedValue<float>((this->valueNormalized.get() == 1.f) ? 0.f : 1.f);
}

void BoolParameter::setValue(bool value)
{
	valueNormalized = BoundrySafeInterpolatedValue<float>(value);
}

const bool BoolParameter::canBeModulated() const
{
	return false;
}

const bool DiscreteParameter::canBeModulated() const
{
	return false;
}

DiscreteParameter::DiscreteParameter(unsigned int numberOfValues, BhString name, BhString unit, const float* values, unsigned int defValueIndex)
	: FloatParameter
		(values != nullptr ? static_cast<float>(defValueIndex) : 0.f
		, NormalizedRange(0.f, static_cast<float>(numberOfValues))
		, name
		, unit)
	, numberOfPossibleValues(numberOfValues)
	, possibleValues(nullptr)
{
	possibleValues = new float[numberOfPossibleValues];
	for (unsigned int i = 0u; i < numberOfPossibleValues; i++)
		if (values != nullptr)
			possibleValues[i] = values[i];
		else
			possibleValues[i] = static_cast<float>(i);
}

DiscreteParameter::~DiscreteParameter()
{
	if (possibleValues != nullptr)
		delete[] possibleValues;
	possibleValues = nullptr;
}

void DiscreteParameter::setToDefaultValue()
{
	valueNormalized = BoundrySafeInterpolatedValue<float>(this->getDefaultValueNormalized());
}

float DiscreteParameter::getValueUnnormalized() const
{
	return possibleValues[static_cast<unsigned int>(getValueNormalized() * static_cast<float>(numberOfPossibleValues))];
};

float DiscreteParameter::getValueUnnormalized()
{
	return possibleValues[static_cast<unsigned int>(getValueNormalized() * static_cast<float>(numberOfPossibleValues))];
};

void DiscreteParameter::setTargetValueUnnormalized(float unnormalizedValue)
{
	float diff = 1000000000.f;
	unsigned int found = 0u;
	for (unsigned int i = 0u; i < numberOfPossibleValues; i++)
	{
		float cdiff = (unnormalizedValue - possibleValues[i]) * (unnormalizedValue - possibleValues[i]);
		if (diff > cdiff)
		{
			diff = cdiff;
			found = i;
		}
	}
	this->setTargetValueNormalized(static_cast<float>(found) / static_cast<float>(numberOfPossibleValues));
};

void DiscreteParameter::setTargetValueNormalized(float normalizedValue)
{
	valueNormalized = BoundrySafeInterpolatedValue<float>(normalizedValue);
}

void DiscreteParameter::next(unsigned int numberOfSamples)
{
	// Empty dummy function, no interpolation is performed for DiscreteParameter
}

const unsigned int DiscreteParameter::getNumberOfPossibleValues() const
{
	return this->numberOfPossibleValues;
}

unsigned int DiscreteParameter::getCurrentNumber() const
{
	// bias for safer rounding
	return static_cast<unsigned int>(getValueNormalized() * static_cast<float>(numberOfPossibleValues) + .25f);
}

float const* DiscreteParameter::getPossibleValue(unsigned int n) const
{
	return &(this->possibleValues[n]);
}

void DiscreteParameter::setDiscreteValue(unsigned int n)
{
	float fraction = static_cast<float>(n) / static_cast<float>(this->getNumberOfPossibleValues());
	this->setTargetValueNormalized(fraction);
}

OptionParameter::OptionParameter(unsigned int numberOfValues, const BhString* names, BhString name, BhString unit)
	:DiscreteParameter(numberOfValues, name, unit)
{
	namesOfOptions = new BhString[numberOfValues];
	for (unsigned int i = 0u; i < numberOfValues; i++)
		namesOfOptions[i] = names[i];
}

OptionParameter::~OptionParameter()
{
	if (namesOfOptions != nullptr)
		delete[] namesOfOptions;
	namesOfOptions = nullptr;
}

BhString OptionParameter::getOptionName(unsigned int num) const
{
	return namesOfOptions[num];
}

void OptionParameter::setTargetValueUnnormalized(float unnormalizedValue)
{
	this->setTargetValueNormalized(
		static_cast<float>(static_cast<unsigned int>(unnormalizedValue) / numberOfPossibleValues)
	);
};
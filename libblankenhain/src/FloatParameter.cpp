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

BoolParameter::BoolParameter(bool defaultValue, BhString name)
	: FloatParameter(static_cast<float>(defaultValue), NormalizedRange(), name, "") {};

void BoolParameter::setTargetValueNormalized(float normalizedValue)
{
	valueNormalized = InterpolatedValue<float>(normalizedValue);
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
	return static_cast<bool>(this->valueNormalized.get());
}

void BoolParameter::flip()
{
	valueNormalized = InterpolatedValue<float>(!static_cast<bool>(this->valueNormalized.get()));
}

void BoolParameter::setValue(bool value)
{
	valueNormalized = InterpolatedValue<float>(value);
}

DiscreteParameter::DiscreteParameter(unsigned int numberOfValues, BhString name, BhString unit, const float* values, unsigned int defValueIndex)
	: FloatParameter(values != nullptr ? values[defValueIndex] : 0.f, NormalizedRange(0u, numberOfValues), name, unit), numberOfPossibleValues(numberOfValues), possibleValues(nullptr)
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
	valueNormalized = InterpolatedValue<float>(this->getDefaultValueNormalized());
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
	valueNormalized = InterpolatedValue<float>(normalizedValue);
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
	return static_cast<unsigned int>(getValueNormalized() * static_cast<float>(numberOfPossibleValues));
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
	this->setTargetValueNormalized(static_cast<unsigned int>(unnormalizedValue) / numberOfPossibleValues);
};
#include "ParameterBundle.h"

ParameterBundle::ParameterBundle(unsigned int numberOfParameters_) : numberOfParameters(numberOfParameters_)
{
	if (numberOfParameters_ != 0u)
	{
		parameters = new FloatParameter*[numberOfParameters];
		ranges = new NormalizedRange[numberOfParameters];
		normalizedValues = new BoundrySafeInterpolatedValue<float>[numberOfParameters];
		canBeModulatedValues = new bool[numberOfParameters];
		for (unsigned int i = 0u; i < this->numberOfParameters; i++)
		{
			parameters[i] = nullptr;
		}
	}
	else
		parameters = nullptr;
}

ParameterBundle::ParameterBundle(const ParameterBundle& other)
	: parameters(other.parameters)
	, numberOfParameters(other.numberOfParameters)
{ }

ParameterBundle::ParameterBundle(ParameterBundle&& other) noexcept /* noexcept needed to enable optimizations in containers */
	: parameters(other.parameters)
	, numberOfParameters(other.numberOfParameters)
{ };

ParameterBundle::~ParameterBundle()
{
	if (parameters != nullptr)
	{
		for (size_t i = 0u; i < numberOfParameters; i++)
		{
			if (getParameter(i) != nullptr) {
				delete getParameter(i);
			}
			parameters[i] = nullptr;
		}

		delete[] parameters;
		delete[] ranges;
		delete[] normalizedValues;
		delete[] canBeModulatedValues;
		parameters = nullptr;
		ranges = nullptr;
		normalizedValues = nullptr;
		canBeModulatedValues = nullptr;
	}
}

FloatParameter* ParameterBundle::getParameter(unsigned int in) const
{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	if (in >= this->numberOfParameters)
		throw ("out of range: number of parameters\n");
#endif
	return parameters[in];
}

void ParameterBundle::initParameter(unsigned int index, FloatParameter* parameter)
{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	if (in >= this->numberOfParameters)
		throw ("out of range: number of parameters\n");
#endif
	parameters[index] = parameter;
	ranges[index] = parameter->range;
	parameter->valueNormalized = &normalizedValues[index];
	normalizedValues[index] = BoundrySafeInterpolatedValue<float>(parameter->getDefaultValueNormalized());
	canBeModulatedValues[index] = parameter->canBeModulated();
}

const unsigned int ParameterBundle::getNumberOfParameters() const
{
	return this->numberOfParameters;
}

const NormalizedRange& ParameterBundle::getRange(unsigned int index) const
{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	if (index >= this->numberOfParameters)
		throw ("out of range: number of parameters\n");
#endif
	return ranges[index];
}

BoundrySafeInterpolatedValue<float>& ParameterBundle::getNormalizedValue(unsigned int index)
{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	if (index >= this->numberOfParameters)
		throw ("out of range: number of parameters\n");
#endif
	return normalizedValues[index];
}

bool ParameterBundle::canBeModulated(unsigned int index)
{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	if (index >= this->numberOfParameters)
		throw ("out of range: number of parameters\n");
#endif
	return canBeModulatedValues[index];
}
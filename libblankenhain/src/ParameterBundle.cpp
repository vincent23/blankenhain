#include "ParameterBundle.h"

ParameterBundle::ParameterBundle(unsigned int numberOfParameters_) : numberOfParameters(numberOfParameters_)
{
	if (numberOfParameters_ != 0u)
	{
		parameters = new FloatParameter*[numberOfParameters];
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
		delete[] *parameters;
		*parameters = nullptr;
	}
}

FloatParameter*& ParameterBundle::getParameter(unsigned int const& in) const
{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS 
	if (in >= this->numberOfParameters)
		throw ("out of range: number of parameters\n");
#endif 
	return (parameters)[in];
}

const unsigned int ParameterBundle::getNumberOfParameters() const
{
	return this->numberOfParameters;
}
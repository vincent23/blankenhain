#include "ParameterBundle.h"
#include <utility>

ParameterBundle::ParameterBundle(unsigned int numberOfParameters_) : numberOfParameters(numberOfParameters_)
{
	FloatParameter** stuff = new FloatParameter*[numberOfParameters];
	parameters = stuff;
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
		delete[] * parameters;
		*parameters = nullptr;
	}
}

unsigned int ParameterBundle::getNumberOfParameters() const
{
	return numberOfParameters;
}

FloatParameter*& ParameterBundle::getParameter(unsigned int const& in) const
{
	return (parameters)[in];
};
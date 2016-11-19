#include "ParameterBundle.h"
#include <utility>

ParameterBundle::ParameterBundle(unsigned int numberOfParameters_) : numberOfParameters(numberOfParameters_)
{
	ParameterWithProperties** stuff = new ParameterWithProperties*[numberOfParameters];
	parameters = stuff;
	bufferScalingValue = 0.f;
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

ParameterWithProperties*& ParameterBundle::getParameter(unsigned int const& in) const
{
	return (parameters)[in];
};
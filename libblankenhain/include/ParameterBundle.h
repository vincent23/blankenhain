#pragma once

#include "FloatParameter.h"

class ParameterBundle
{
public:
	ParameterBundle(unsigned int numberOfParameters_);
	ParameterBundle(const ParameterBundle& other);
	ParameterBundle(ParameterBundle&& other) noexcept;
	virtual ~ParameterBundle();

	unsigned int getNumberOfParameters() const;

	ParameterWithProperties*& getParameter(unsigned int const& in) const;

	float bufferScalingValue;
private:
	const unsigned int numberOfParameters;
	ParameterWithProperties** parameters;
};
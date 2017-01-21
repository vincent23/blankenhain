#pragma once

#include "FloatParameter.h"
#pragma once
/// Contains a number of FloatParameters
class ParameterBundle
{
public:
	ParameterBundle(unsigned int numberOfParameters_);
	ParameterBundle(const ParameterBundle& other);
	ParameterBundle(ParameterBundle&& other) noexcept;
	virtual ~ParameterBundle();

	const unsigned int getNumberOfParameters() const;

	FloatParameter*& getParameter(unsigned int const& in) const;
private:
	const unsigned int numberOfParameters;
	FloatParameter** parameters;
};
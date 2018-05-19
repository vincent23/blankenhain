#pragma once

#include "FloatParameter.h"

/// Contains a number of FloatParameters
class ParameterBundle
{
public:
	ParameterBundle(unsigned int numberOfParameters_);
	ParameterBundle(const ParameterBundle& other);
	ParameterBundle(ParameterBundle&& other) noexcept;
	virtual ~ParameterBundle();

	const unsigned int getNumberOfParameters() const;

	FloatParameter* getParameter(unsigned int in) const;
	void initParameter(unsigned int index, FloatParameter* parameter);

	const NormalizedRange& getRange(unsigned int index) const;
	BoundrySafeInterpolatedValue<float>& getNormalizedValue(unsigned int index);
	bool canBeModulated(unsigned int index);

private:
	const unsigned int numberOfParameters;
	FloatParameter** parameters;
	NormalizedRange* ranges;
	bool* canBeModulatedValues;
	BoundrySafeInterpolatedValue<float>* normalizedValues;
};
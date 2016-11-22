#pragma once

#include <string>
#include "NormalizedRange.h"

using String = std::string;

class FloatParameter : public NormalizedRange
{
public:
	FloatParameter(float defaultValueUnnormalized, const NormalizedRange& range, String name, String unit);

	float getDefaultValueNormalized() const;
	float getValueNormalized() const;
	float getValueUnnormalized() const;
	String getName(unsigned int maximumStringLength = 0u) const;
	String getUnit() const;

	void setTargetValueNormalized(float normalizedValue);
	void setTargetValueUnnormalized(float unnormalizedValue);
	void next(unsigned int numberOfSamples = 1);

private:
	float defaultValueNormalized;
	// currentValue has to come after defaultValue because of initalization order
	float currentValueNormalized;

	float interpolationStepSize = 0;
	unsigned int interpolationCounter = 0;

	String unit;
	String name;
};
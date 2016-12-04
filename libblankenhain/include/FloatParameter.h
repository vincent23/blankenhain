#pragma once

#include <string>
#include "NormalizedRange.h"

using BhString = std::string;

class FloatParameter : public NormalizedRange
{
public:
	FloatParameter(float defaultValueUnnormalized, const NormalizedRange& range, BhString name, BhString unit);

	float getDefaultValueNormalized() const;
	float getValueNormalized() const;
	float getValueUnnormalized() const;
	BhString getName(unsigned int maximumStringLength = 0u) const;
	BhString getUnit() const;

	void setTargetValueNormalized(float normalizedValue);
	void setTargetValueUnnormalized(float unnormalizedValue);
	void next(unsigned int numberOfSamples = 1);

private:
	float defaultValueNormalized;
	// currentValue has to come after defaultValue because of initalization order
	float currentValueNormalized;

	float interpolationStepSize = 0;
	unsigned int interpolationCounter = 0;

	BhString unit;
	BhString name;
};
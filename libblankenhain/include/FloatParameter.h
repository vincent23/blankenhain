#pragma once

#include <string>
#include "NormalizedRange.h"
#include "InterpolatedValue.h"

using BhString = std::string;


/// Stores a float value, maps to a normalized range (between 0.f and 1.f) from
/// an arbitrary range given during construction. Is also able to perform interpolation
/// via setTargetValue() and next() functions. Stores name and unit of the parameter.
class FloatParameter : public NormalizedRange
{
public:
	FloatParameter(float defaultValueUnnormalized, const NormalizedRange& range, BhString name, BhString unit);

	float getDefaultValueNormalized() const;
	float getValueNormalized() const;
	float getValueNormalized();

	float getValueUnnormalized() const;
	float getValueUnnormalized();

	BhString getName(unsigned int maximumStringLength = 0u) const;
	BhString getUnit() const;

	void setTargetValueNormalized(float normalizedValue);
	void setTargetValueUnnormalized(float unnormalizedValue);
	void next(unsigned int numberOfSamples = 1);

	virtual bool isBoolean() const
	{
		return false;
	}

private:
	float defaultValueNormalized;
	BhString unit;
	BhString name;
	InterpolatedValue<float> valueNormalized;
};

class BoolParameter : public FloatParameter
{
public:
	BoolParameter(bool defaultValue, BhString name)
		: FloatParameter(static_cast<float>(defaultValue), NormalizedRange(), name, "") {};
	virtual bool isBoolean() const final { return true; }
};
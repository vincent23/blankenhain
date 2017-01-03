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

	virtual float getValueUnnormalized() const;
	virtual float getValueUnnormalized();

	BhString getName(unsigned int maximumStringLength = 0u) const;
	BhString getUnit() const;

	virtual void setTargetValueNormalized(float normalizedValue);
	virtual void setTargetValueUnnormalized(float unnormalizedValue);
	virtual void next(unsigned int numberOfSamples = 1);

protected:
	InterpolatedValue<float> valueNormalized;

private:
	float defaultValueNormalized;
	BhString unit;
	BhString name;

};

class BoolParameter : public FloatParameter
{
public:
	BoolParameter(bool defaultValue, BhString name)
		: FloatParameter(static_cast<float>(defaultValue), NormalizedRange(), name, "") {};
	virtual void setTargetValueNormalized(float normalizedValue) override
	{
		valueNormalized = InterpolatedValue<float>(normalizedValue);
	}
	virtual void next(unsigned int numberOfSamples) override
	{
		// no interpolation for IntegerParam
	}
};

class IntegerParameter : public FloatParameter
{
	IntegerParameter(const float*& values, unsigned int numberOfValues, BhString name, BhString unit)
		: FloatParameter(values[0], NormalizedRange(), name, unit), numberOfPossibleValues(numberOfValues), possibleValues(nullptr)
	{
		possibleValues = new float[numberOfPossibleValues];
		for (unsigned int i = 0u; i < numberOfPossibleValues; i++)
			possibleValues[i] = values[i];
	}

	~IntegerParameter()
	{
		if (possibleValues != nullptr)
			delete[] possibleValues;
		possibleValues = nullptr;
	}

	virtual float getValueUnnormalized() const override
	{
		return possibleValues[static_cast<unsigned int>(getValueNormalized() * static_cast<float>(numberOfPossibleValues))];
	};

	virtual float getValueUnnormalized() override
	{
		return possibleValues[static_cast<unsigned int>(getValueNormalized() * static_cast<float>(numberOfPossibleValues))];
	};

	virtual void setTargetValueUnnormalized(float unnormalizedValue)
	{
		float diff = 1000000000.f;
		unsigned int found = 0u;
		for (unsigned int i = 0u; i < numberOfPossibleValues; i++)
		{
			float cdiff = (unnormalizedValue - possibleValues[i]) * (unnormalizedValue - possibleValues[i]);
			if (diff > cdiff)
			{
				diff = cdiff;
				found = i;
			}
		}
		this->setTargetValueNormalized(static_cast<float>(found) / static_cast<float>(numberOfPossibleValues));

		// No interpolation for IntegerParameters
		//next(2147483647u); //maxint via https://msdn.microsoft.com/de-de/library/296az74e.aspx
	};

	virtual void setTargetValueNormalized(float normalizedValue) override
	{
		valueNormalized = InterpolatedValue<float>(normalizedValue);
	}

	virtual void next(unsigned int numberOfSamples) override
	{
		// no interpolation for IntegerParam
	}
private:
	float *possibleValues;
	const unsigned int numberOfPossibleValues;
};
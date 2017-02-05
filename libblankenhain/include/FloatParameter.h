#pragma once

#include <string>
#include "NormalizedRange.h"
#include "InterpolatedValue.h"

using BhString = std::string;


/// Stores a float value, maps to a normalized range (between 0.f and 1.f) from
/// an arbitrary range given during construction. Normalized values are always in the range [0 <= value <=1]. Is also able to perform interpolation
/// via setTargetValue() and next() functions. Stores name and unit of the parameter.
class FloatParameter : public NormalizedRange
{
public:
	FloatParameter(float defaultValueUnnormalized, const NormalizedRange& range, BhString name, BhString unit);

	float getDefaultValueNormalized() const;

	/*
	 * Interpolates to default value.
	 */
	virtual void setToDefaultValue();


	float getValueNormalized() const;
	float getValueNormalized();

	BhString getName(unsigned int maximumStringLength = 0u) const;
	BhString getUnit() const;

	virtual void setTargetValueNormalized(float normalizedValue);
	virtual void setTargetValueUnnormalized(float unnormalizedValue);
	virtual void next(unsigned int numberOfSamples = 1);
	/**
	 * Interpolates to default value.
	 */

	virtual float getValueUnnormalized() const;
	virtual float getValueUnnormalized();

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

	virtual void setTargetValueUnnormalized(float unnormalizedValue) override
	{
		this->setTargetValueNormalized(unnormalizedValue);
	}

	//

	bool getValue() const
	{
		return static_cast<bool>(this->valueNormalized.get());
	}

	void flip()
	{
		valueNormalized = InterpolatedValue<float>(! static_cast<bool>(this->valueNormalized.get()));
	}

	void setValue(bool value)
	{
		valueNormalized = InterpolatedValue<float>(value);
	}

};

// No interpolation here
class DiscreteParameter : public FloatParameter
{
public:
	DiscreteParameter(unsigned int numberOfValues, BhString name = "", BhString unit = "", const float* values = nullptr, unsigned int defValueIndex = 0u)
		: FloatParameter(values != nullptr ? values[defValueIndex] : 0.f, NormalizedRange(0u, numberOfValues), name, unit), numberOfPossibleValues(numberOfValues), possibleValues(nullptr)
	{
		possibleValues = new float[numberOfPossibleValues];
		for (unsigned int i = 0u; i < numberOfPossibleValues; i++)
			if (values != nullptr)
				possibleValues[i] = values[i];
			else
				possibleValues[i] = static_cast<float>(i);
	}

	~DiscreteParameter()
	{
		if (possibleValues != nullptr)
			delete[] possibleValues;
		possibleValues = nullptr;
	}

	virtual void setToDefaultValue()
	{
		valueNormalized = InterpolatedValue<float>(this->getDefaultValueNormalized());
	}

	virtual float getValueUnnormalized() const override
	{
		return possibleValues[static_cast<unsigned int>(getValueNormalized() * static_cast<float>(numberOfPossibleValues))];
	};

	virtual float getValueUnnormalized() override
	{
		return possibleValues[static_cast<unsigned int>(getValueNormalized() * static_cast<float>(numberOfPossibleValues))];
	};

	virtual void setTargetValueUnnormalized(float unnormalizedValue) override
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
	};

	virtual void setTargetValueNormalized(float normalizedValue) override
	{
		valueNormalized = InterpolatedValue<float>(normalizedValue);
	}

	virtual void next(unsigned int numberOfSamples) final
	{
		// no interpolation for IntegerParam
	}

	//

	const unsigned int getNumberOfPossibleValues() const
	{
		return this->numberOfPossibleValues;
	}

	unsigned int getCurrentNumber() const
	{
		return static_cast<unsigned int>(getValueNormalized() * static_cast<float>(numberOfPossibleValues));
	}

	float const* getPossibleValue(unsigned int n) const
	{
		return &(this->possibleValues[n]);
	}

	float* getPossibleValue(unsigned int n)
	{
		return &(this->possibleValues[n]);
	}

protected:
	const unsigned int numberOfPossibleValues;
private:
	float *possibleValues;
};

class OptionParameter : public DiscreteParameter
{
public:
	OptionParameter(unsigned int numberOfValues, const BhString* names, BhString name = "", BhString unit = "")
		:DiscreteParameter(numberOfValues, name, unit)
	{
		namesOfOptions = new BhString[numberOfValues];
		for (unsigned int i = 0u; i < numberOfValues; i++)
				namesOfOptions[i] = names[i];
	}

	~OptionParameter()
	{
		if (namesOfOptions != nullptr)
			delete[] namesOfOptions;
		namesOfOptions = nullptr;
	}

	BhString getOptionName(unsigned int num) const
	{
		return namesOfOptions[num];
	}

	virtual void setTargetValueUnnormalized(float unnormalizedValue) final
	{
		this->setTargetValueNormalized(static_cast<unsigned int>(unnormalizedValue) / numberOfPossibleValues);
	};

private:
	BhString* namesOfOptions;
};
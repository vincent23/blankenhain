#pragma once

#include <string>
#include "NormalizedRange.h"
#include "InterpolatedValue.h"

using BhString = std::string;


/// Stores a float value, maps to a normalized range (between 0.f and 1.f) from
/// an arbitrary range given during construction. Normalized values are always in the range [0 <= value <=1]. Is also able to perform interpolation
/// via setTargetValue() and next() functions. Stores name and unit of the parameter.
/// Unnormalized Values are according to the specified range
class FloatParameter : public NormalizedRange
{
public:
	FloatParameter(float defaultValueUnnormalized, const NormalizedRange& range, BhString name, BhString unit);


	// These function may reasonably be used for FloatParam
	// and all its children
	float getDefaultValueNormalized() const;
	BhString getName(unsigned int maximumStringLength = 0u) const;
	BhString getUnit() const;

	// This may be overriden by children to prevent interpolation
	virtual void setToDefaultValue();


	// For classes derived from FloatParam
	// you should prefer using their custom
	// functions to change the param rather than these
	float getValueNormalized() const;
	float getValueNormalized();
	virtual float getValueUnnormalized() const;
	virtual float getValueUnnormalized();
	virtual void setTargetValueNormalized(float normalizedValue);
	virtual void setTargetValueUnnormalized(float unnormalizedValue);

	// Performs interpolation. Either call this after every
	// usage of a param if you need VERY current and accurate 
	// param values. If more-or-less-accurate parameter values are
	// also fine for you, you can call EffectBase::nextSample(unsigned int steps)
	// at the end of the process function. This will take care of
	// parameter interpolation.
	virtual void next(unsigned int numberOfSamples = 1);


protected:
	InterpolatedValue<float> valueNormalized;

private:
	float defaultValueNormalized;
	BhString unit;
	BhString name;

};

/**
 * Parameter for a Boolean Value.
 * Prefered access is via the functions:
 * flip()
 * setValue(bool)
 * bool getValue()
 *
 * Normalized range is euqal to unnormalized range [0,1] 
 *
 * Try not to explicitly use setTargetValueNormalized, getValueUnnormalized and so on...
 */
class BoolParameter : public FloatParameter
{
public:
	BoolParameter(bool defaultValue, BhString name);

	virtual void setTargetValueNormalized(float normalizedValue) override;
	virtual void next(unsigned int numberOfSamples) final;
	virtual void setTargetValueUnnormalized(float unnormalizedValue) final;


	bool getValue() const;
	void flip();
	void setValue(bool value);

};

/**
 * A DiscreteParameter can only have discrete numerical values.
 * No interpolation of the values will be performed, as they are discrete.
 *
 * Example: "Number of iterations" [0,1,2,...,8,9]
 * Example: "Multiplier" [0.125, 0.25, 0.50, 1.0, 2.0, 4.0]
 * In these cases it is more reasonable to use a DiscreteParameter instead of a FloatParameter.
 *
 * 
 * @param numberOfValues: Number of discrete Values possible
 * @param name: Name of the Parameter
 * @param unit: Unit of the parameter (for example milliseconds or Hz)
 * @param values: Array containing the discrete values the parameter may be set to as floats
 * @param desValueIndex: Index of the default value of the parameter as its place in the float-array "values".
 *
 * NOTE: If no float* values is specified or nullptr is passed in during construction, a default DiscreteParameter will be constructed-
 * It will contain integers counting up from 0 up to numberOfValues.
 * Example: numberOfValues == 5u; values == nullptr: [0.f,1.f,2.f,3.f,4.f]
 * 
 */
class DiscreteParameter : public FloatParameter
{
public:
	DiscreteParameter(unsigned int numberOfValues, BhString name, BhString unit = "", const float* values = nullptr, unsigned int defValueIndex = 0u);

	~DiscreteParameter();

	virtual void setToDefaultValue() final;
	virtual float getValueUnnormalized() const final;
	virtual float getValueUnnormalized() final;
	virtual void setTargetValueNormalized(float normalizedValue) final;
	virtual void next(unsigned int numberOfSamples) final;

	/// THIS FUNCTION SHOULD NEVER BE CALLED!!!!
	/// It matches the input float to the possible discrete values
	/// By finding the one with the minimum mean squared error.
	/// This is very slow and horrible and you really should never call this
	/// for DiscreteParameter
	virtual void setTargetValueUnnormalized(float unnormalizedValue) override;


	// Preferred access to DiscreteParameter via these functions.
	const unsigned int getNumberOfPossibleValues() const;
	unsigned int getCurrentNumber() const;
	float const* getPossibleValue(unsigned int n) const;
	void setDiscreteValue(unsigned int n);

protected:
	const unsigned int numberOfPossibleValues;
private:
	float *possibleValues;
};

/**
 * An OptionParameter is a DiscreteParameter which has a certain name (string)
 * associated to every discrete value, where the numerical discrete values do not matter.
 * Example:
 * Waveform: [Sine (=0), Square(=1), Triangle(=2), Saw(=3)]
 *
 * It can be accessed just like a DiscreteParameter. The difference mainly matters during
 * rendering of the GUI, where an OptionParameter would for example be a drop-down menu
 * of strings and a DiscreteParameter a discrete Slider.
 */
class OptionParameter : public DiscreteParameter
{
public:
	OptionParameter(unsigned int numberOfValues, const BhString* names, BhString name, BhString unit = "");

	~OptionParameter();

	BhString getOptionName(unsigned int num) const;

	virtual void setTargetValueUnnormalized(float unnormalizedValue) final;

private:
	BhString* namesOfOptions;
};
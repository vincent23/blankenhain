#pragma once

#include <string>
#include "NormalizedRange.h"

using String = std::string;

// Internally all floats should be normalized, since
// this is necessary for communication with DAW-Host.
// Therefore, you need to specify NormalizedRanges
// For your stuff in order to get unnormalized values
class FloatParameter
{
public:
	// USE THIS CONSTRUCTOR
	FloatParameter(float defaultValueUnnormalized);

	// These are overwritten and will be called by the DAW host
	// The values are expected to be normalized
	float getTargetValueNormalized() const;
	float getOldValueNormalized() const;
	void setTargetValueNormalized(float const& in);
	void setOldValueNormalized(float const& in);

	float getInterpolatedNormmalized(size_t currentStep, size_t maxSteps);
	float getDefaultValueNormalized() const;

protected:
	float oldValueNormalized;
	float targetValueNormalized;
	float normalizedDefaultValue;
};

class ParameterWithProperties :
	public FloatParameter,
	public NormalizedRange
{
public:
	ParameterWithProperties(
		float defaultValueUnnormalized,
		NormalizedRange const& range,
		String name, String unit);

	float getInterpolatedUnnormmalized(size_t currentStep, size_t maxSteps) const;

	String getName(unsigned int maximumStringLength = 0u) const;

	float getDefaultValueUnnormalized() const;

	std::string getUnit() const;

	float getImmediateValueAndUpdateUnnormalized();
	float getImmediateValueAndUpdateNormalized();
	float getImmediateValueNormalized();
	float getImmediateValueUnnormalized();
	void setImmediateValueNormalized(float in);
	void setImmediateValueUnnormalized(float in);

	// Now, for the functions taking unnormalized values.
	// Checks will be performed if they are in range if
	// #BLANKENHAIN_CHECKS is enabled.
	float getTargetValueUnnormalized() const;
	void  setTargetValueUnnormalized(float unnormalized);

	// For sofisticated interpolation stuff, not
	// used to much right now
	float getOldValueUnnormalized() const;
	void  setOldValueUnnormalized(float unnormalizedOldValue_);

	void setInterpolationDistance(size_t const& dist);
	void resetInterpolationCounter();

	bool oldAndTargetValueMatch() const;

	ParameterWithProperties& operator=(const ParameterWithProperties & in) = default;
private:
	float incrementImmediateValueAndReturnOldValueNormalized();
	String unit;
	// This Value should be between 0 and 1 and specifies the end of
	// linear interpolation in terms of "percentage of whole buffer"

	String name;
	float immediateValueNormalized;

	unsigned int interpolationDistance;

	unsigned int interpolationIteration;
};


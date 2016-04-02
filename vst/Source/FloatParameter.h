#pragma once

#include <juce>
#include <functional>

// Internally all floats should be normalized, since 
// this is necessary for communication with DAW-Host.
// Therefore, you need to specify NormalizedRanges
// For your stuff in order to get unnormalized values

class FloatParameter : public AudioProcessorParameter
{
public:
	// USE THIS CONSTRUCTOR
	FloatParameter(float defaultValue, const String& name = "I was not initialized properly. U bad boy mhmhm.", \
		float bufferScalingValue_ = 1.f, const NormalizedRange& normalizedRange = NormalizedRange());
	// COPY BY REFERENCE
	FloatParameter(const FloatParameter&);

  // These are overwritten and will be called by the DAW host
  // The values are expected to be normalized
	float getValue() const override;
	void setValue(float normalizedNewValue_) override;
  float getDefaultValue() const override;

  // Now, for the functions taking unnormalized values.
  // Checks will be performed if they are in range if
  // #BLANKENHAIN_CHECKS is enabled.
  float getUnnormalizedValue() const;
  void setUnnormalizedValue(float unnormalized);

  // For sofisticated interpolation stuff, not
  // used to much right now
	float getUnnormalizedOldValue() const;
	void setUnnormalizedOldValue(float unnormalizedOldValue_);
	void setUnnormalizedOldValue(void);

	float getBufferScalingValue() const;

	String getName(int maximumStringLength) const override;
	String getLabel() const override;
	float getValueForText(const String &text) const override;

	float getNormalizedValue() const;
	float getNormalizedOldValue() const;
	float getNormalizedDefaultValue() const;

private:
	const float defaultValue;
	NormalizedRange range;
	float value;
	float oldValue;
	const float bufferScalingValue;
	// This Value should be between 0 and 1 and specifies the end of
	// linear interpolation in terms of "percentage of whole buffer"
	
	const String name; 
};
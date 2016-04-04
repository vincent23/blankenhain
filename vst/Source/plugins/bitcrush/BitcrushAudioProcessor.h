#pragma once

#include <juce>
#include "BlankenhainAudioProcessor.h"
#include "FloatParameter.h"

class BitcrushAudioProcessor : public BlankenhainAudioProcessor
{
public:
	BitcrushAudioProcessor();

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;

	void setBitcrush(float bitcrush);
	void setDownsample(float downsample);
	void setWet(float wet);

	float getBitcrush() const;
	float getDownsample() const;
	float getWet() const;

protected:
	var getState() override;
	void setState(const var& state) override;

	FloatParameter* bitcrush;
	FloatParameter* downsample;
	FloatParameter* wet;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BitcrushAudioProcessor)
};

// Channel number is expected to be == 2
// All parameter values between 0 and 1
void processBitcrush(Sample* data, size_t numberOfSamples, float bitcrush, float downsample,
  float wet);

// Casts value of sample to int (discretization)
// and back to double
Sample discretize(Sample const& sample);

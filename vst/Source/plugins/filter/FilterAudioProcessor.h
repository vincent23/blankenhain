#pragma once

#include <juce>
#include "BlankenhainAudioProcessor.h"
#include "FloatParameter.h"

class FilterAudioProcessor : public BlankenhainAudioProcessor
{
public:
	FilterAudioProcessor();

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;

	void setResonance(float resonance);
	void setFrequency(float frequency);
	void setFilterType(int index);
	float getResonance();
	float getFrequency();
	int getFilterType();

protected:
	var getState() override;
	void setState(const var& state) override;

private:
	FloatParameter* resonance;
	FloatParameter* frequency;
	enum FilterType {
		High = 0,
		Low,
		Band,
		Notch,
	} filterType;

	Filter* filter = new Filter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterAudioProcessor)
};

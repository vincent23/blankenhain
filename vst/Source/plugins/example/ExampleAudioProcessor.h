#pragma once

#include <juce>
#include "BlankenhainAudioProcessor.h"

class ExampleAudioProcessor : public BlankenhainAudioProcessor
{
public:
	ExampleAudioProcessor();

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;

protected:
	var getState() override;
	void setState(const var& state) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ExampleAudioProcessor)
};

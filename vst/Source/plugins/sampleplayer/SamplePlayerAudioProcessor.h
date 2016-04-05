#pragma once

#include <juce>
#include "BlankenhainAudioProcessor.h"
#include <memory>

class SamplePlayerAudioProcessor : public BlankenhainAudioProcessor
{
public:
	SamplePlayerAudioProcessor();

	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

	void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

	AudioBuffer<float>& getSampleBuffer();

	AudioProcessorEditor* createEditor() override;

protected:
	var getState() override;
	void setState(const var& state) override;

private:
	AudioBuffer<float> sampleBuffer;
	int position = 0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplePlayerAudioProcessor)
};
#pragma once

#include <juce>
#include "BlankenhainAudioProcessor.h"

class CompressorAudioProcessor : public BlankenhainAudioProcessor
{
public:
	CompressorAudioProcessor();

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;

protected:
	var getState() override;
	void setState(const var& state) override;

private:
	float envelope[2] = {};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CompressorAudioProcessor)
};

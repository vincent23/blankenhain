#pragma once

#include <juce>
#include "BlankenhainAudioProcessor.h"
#include "FloatParameter.h"
#include "BoolParameter.h"

class KickAudioProcessor : public BlankenhainAudioProcessor
{
public:
	KickAudioProcessor();

	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

	void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

	AudioProcessorEditor* createEditor() override;

protected:
	var getState() override;
	void setState(const var& state) override;

private:
	KickSynth kickSynth;
	Sample* internalBuffer = new Sample[16];

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KickAudioProcessor)
};

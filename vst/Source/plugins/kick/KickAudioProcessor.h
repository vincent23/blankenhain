#pragma once

#include <juce>
#include "BlankenhainAudioProcessor.h"
#include "FloatParameter.h"
#include "BoolParameter.h"
#include <memory>

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
	std::unique_ptr<KickSynth> kickSynth = std::unique_ptr<KickSynth>(new KickSynth());
	std::unique_ptr<Sample> internalBuffer = std::unique_ptr<Sample>(new Sample[16]);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KickAudioProcessor)
};

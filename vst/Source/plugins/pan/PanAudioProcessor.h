#pragma once

#include <juce>
#include "BlankenhainAudioProcessor.h"
#include "FloatParameter.h"
#include "BoolParameter.h"

class PanAudioProcessor : public BlankenhainAudioProcessor
{
public:
	PanAudioProcessor();

	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

	void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

	AudioProcessorEditor* createEditor() override;

	void setPanning(FloatParameter);
	float getPanning();

protected:
	var getState() override;
	void setState(const var& state) override;

private:
	FloatParameter *panning;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PanAudioProcessor)
};

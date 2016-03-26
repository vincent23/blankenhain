#pragma once

#include <juce>
#include "BlankenhainAudioProcessor.h"
#include "FloatParameter.h"
#include "CircularBuffer.h"

class LimiterAudioProcessor : public BlankenhainAudioProcessor
{
public:
	LimiterAudioProcessor();

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;

    void setLookahead(FloatParameter lookahead);
    void setThreshold(FloatParameter threshold);
    void setRelease(FloatParameter release);

    float getLookahead();
    float getRelease();
    float getThreshold();
protected:
	var getState() override;
	void setState(const var& state) override;

private:
  FloatParameter *release;
  FloatParameter *lookahead;
  FloatParameter *threshold;
  CircularBuffer<float>* longCircularBuffer = nullptr;
  CircularBuffer<float>* shortCircularBuffer1 = nullptr;
  CircularBuffer<float>* shortCircularBuffer2 = nullptr;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LimiterAudioProcessor)
};

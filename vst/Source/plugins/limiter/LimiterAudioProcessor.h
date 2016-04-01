#pragma once

#include <juce>
#include "BlankenhainAudioProcessor.h"
#include "FloatParameter.h"
#include "CircularBuffer.h"
#include "BoolParameter.h"

class LimiterAudioProcessor : public BlankenhainAudioProcessor
{
public:
	LimiterAudioProcessor();

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;

    void setLookahead(FloatParameter lookahead);
    void setRelease(FloatParameter release);
    void setOgain(FloatParameter ogain);
    void setIgain(FloatParameter igain);
    void switchBypass();

    float getLookahead();
    float getRelease();
    float getOgain();
    float getIgain();
    bool getBypass();
    float suggestedOgain;
protected:
	var getState() override;
	void setState(const var& state) override;

private:
  FloatParameter *release = nullptr;
  FloatParameter *lookahead = nullptr;
  FloatParameter *ogain = nullptr;
  FloatParameter *igain = nullptr;
  BoolParameter *bypass = nullptr;;
  CircularBuffer<float>* longCircularBuffer = nullptr;
  CircularBuffer<float>* shortCircularBuffer1 = nullptr;
  CircularBuffer<float>* shortCircularBuffer2 = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LimiterAudioProcessor)
};

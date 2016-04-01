#pragma once

#include <juce>
#include "BlankenhainAudioProcessor.h"
#include "FloatParameter.h"
#include "BoolParameter.h"
#include "CircularBuffer.h"

class CompressorAudioProcessor : public BlankenhainAudioProcessor
{
public:
	CompressorAudioProcessor();

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;

    void setRatio(float ratio);
    void setThreshold(float threshold);
    void setAttack(float attack);
    void setRelease(float release);
    void setLimiter(bool value);

    float getRatio();
    float getRelease();
    float getThreshold();
    float getAttack();
    bool getLimiter();
protected:
	var getState() override;
	void setState(const var& state) override;

private:
	float envelope[2] = {};
  FloatParameter *release;
  FloatParameter *attack;
  FloatParameter *ratio;
  FloatParameter *threshold;
  BoolParameter *limiterOn;
  CircularBuffer<float> delayLine;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CompressorAudioProcessor)
};

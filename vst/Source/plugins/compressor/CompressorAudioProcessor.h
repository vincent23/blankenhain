#pragma once

#include <juce>
#include "BlankenhainAudioProcessor.h"
#include "FloatParameter.h"

class CompressorAudioProcessor : public BlankenhainAudioProcessor
{
public:
	CompressorAudioProcessor();

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;

    void setRatio(FloatParameter ratio);
    void setThreshold(FloatParameter threshold);
    void setAttack(FloatParameter attack);
    void setRelease(FloatParameter release);
    void setPostgain(FloatParameter postgain);

    float getRatio();
    float getRelease();
    float getThreshold();
    float getAttack();
    float getPostgain();
protected:
	var getState() override;
	void setState(const var& state) override;

private:
	float envelope[2] = {};
  FloatParameter *release;
  FloatParameter *attack;
  FloatParameter *ratio;
  FloatParameter *threshold;
  FloatParameter *postgain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CompressorAudioProcessor)
};

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

    void setRatio(float ratio);
    void setThreshold(float threshold);
    void setAttack(float attack);
    void setRelease(float release);
    void setPostgain(float postgain);

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

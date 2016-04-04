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
	Sample* envelope;
  FloatParameter *release;
  FloatParameter *attack;
  FloatParameter *ratio;
  FloatParameter *threshold;
  BoolParameter *limiterOn;
  CircularBuffer<Sample> delayLine;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CompressorAudioProcessor)
};

void processCompressor(Sample* data, size_t numberOfSamples,
  float release, float attack, float ratio, float threshold,
  bool limiterOn,
  CircularBuffer<Sample>& delayLine, Sample& envelope);
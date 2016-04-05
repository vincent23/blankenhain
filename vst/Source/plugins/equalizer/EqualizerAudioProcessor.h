/*
  ==============================================================================

    EqualizerAudioProcessor.h
    Created: 1 Apr 2016 8:04:43pm
    Author:  Dustin Kaiser

  ==============================================================================
*/

#ifndef EQUALIZERAUDIOPROCESSOR_H_INCLUDED
#define EQUALIZERAUDIOPROCESSOR_H_INCLUDED
#pragma once

#include <juce>
#include "BlankenhainAudioProcessor.h"
#include "FloatParameter.h"

class EqualizerAudioProcessor : public BlankenhainAudioProcessor
{
public:
  EqualizerAudioProcessor();

  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;
  void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

  AudioProcessorEditor* createEditor() override;

  void setLow(float value);
  void setMid(float value);
  void setHigh(float value);
  void setLowFreq(float value);
  void setHighFreq(float value);

  float getLow();
  float getMid();
  float getHigh();
  float getLowFreq();
  float getHighFreq();

protected:
  var getState() override;
  void setState(const var& state) override;

private:
  FloatParameter* low;
  FloatParameter* mid;
  FloatParameter* high;
  FloatParameter* lowFreq;
  FloatParameter* highFreq;
  effects::equalizer::Sample_EQSTATE* es;
  const double vsa = (1.0 / 4294967295.0); // Very small amount (Denormal Fix)

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EqualizerAudioProcessor)
};



#endif  // EQUALIZERAUDIOPROCESSOR_H_INCLUDED

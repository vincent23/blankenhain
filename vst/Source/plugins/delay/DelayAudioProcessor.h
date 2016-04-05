/*
  ==============================================================================

    DelayAudioProcessor.h
    Created: 1 Apr 2016 8:04:43pm
    Author:  Dustin Kaiser

  ==============================================================================
*/

#ifndef DELAYAUDIOPROCESSOR_H_INCLUDED
#define DELAYAUDIOPROCESSOR_H_INCLUDED
#pragma once

#include <juce>
#include "BlankenhainAudioProcessor.h"
#include "FloatParameter.h"

class DelayAudioProcessor : public BlankenhainAudioProcessor
{
public:
  DelayAudioProcessor();

  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;
  void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

  AudioProcessorEditor* createEditor() override;

  void setLength(float value);
  void setPan(float value);
  void setFeedback(float value);
  void setDrywet(float value);

  float getLength() const;
  float getPan() const;
  float getFeedback() const;
  float getDrywet() const;
  float getBPM() const;

protected:
  var getState() override;
  void setState(const var& state) override;

private:
  FloatParameter* length;
  FloatParameter* pan;
  FloatParameter* feedback;
  FloatParameter* drywet;
  CircularBuffer<Sample> delayline;
  float bpm = 120.f;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayAudioProcessor)
};



#endif  // DELAYAUDIOPROCESSOR_H_INCLUDED

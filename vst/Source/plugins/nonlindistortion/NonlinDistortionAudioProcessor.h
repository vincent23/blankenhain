/*
  ==============================================================================

    NonlinDistortionAudioProcessor.h
    Created: 1 Apr 2016 8:04:43pm
    Author:  Dustin Kaiser

  ==============================================================================
*/

#ifndef NONLINDISTORTIONAUDIOPROCESSOR_H_INCLUDED
#define NONLINDISTORTIONAUDIOPROCESSOR_H_INCLUDED
#pragma once

#include <juce>
#include "BlankenhainAudioProcessor.h"
#include "FloatParameter.h"

class NonlinDistortionAudioProcessor : public BlankenhainAudioProcessor
{
public:
  NonlinDistortionAudioProcessor();

  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;
  void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

  AudioProcessorEditor* createEditor() override;

  void setAlg(size_t index);
  size_t getAlg() const;
  size_t getIterations() const;
  float getIngain() const;
  void setIterations(size_t in);
  void setIngain(float in);

protected:
  var getState() override;
  void setState(const var& state) override;

private:
  FloatParameter* ingaindist;
  FloatParameter* iterations;
  enum Algorithm {
    none = 0,
    Araya_Suyama,
    Doidic_sym,
    Doidic_asym,
  } alg;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NonlinDistortionAudioProcessor)
};



#endif  // NONLINDISTORTIONAUDIOPROCESSOR_H_INCLUDED

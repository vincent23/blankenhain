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

  typedef struct
  {
    // Filter #1 (Low band)

    double lf = 0.; // Frequency
    double f1p0 = 0.; // Poles ...
    double f1p1 = 0.;
    double f1p2 = 0.;
    double f1p3 = 0.;

    // Filter #2 (High band)

    double hf = 0.; // Frequency
    double f2p0 = 0.; // Poles ...
    double f2p1 = 0.;
    double f2p2 = 0.;
    double f2p3 = 0.;

    // Sample history buffer

    double sdm1 = 0.; // Sample data minus 1
    double sdm2 = 0.; // 2
    double sdm3 = 0.; // 3

    // Gain Controls

    double lg = 0.; // low gain
    double mg = 0.; // mid gain
    double hg = 0.; // high gain

  } EQSTATE;
  EQSTATE *es;
  double do_3band(double sample);
  void init_3band_state(int lowfreq, int highfreq, int mixfreq);

protected:
  var getState() override;
  void setState(const var& state) override;

private:
  FloatParameter* low;
  FloatParameter* mid;
  FloatParameter* high;
  FloatParameter* lowFreq;
  FloatParameter* highFreq;
  const double vsa = (1.0 / 4294967295.0); // Very small amount (Denormal Fix)

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EqualizerAudioProcessor)
};





#endif  // EQUALIZERAUDIOPROCESSOR_H_INCLUDED

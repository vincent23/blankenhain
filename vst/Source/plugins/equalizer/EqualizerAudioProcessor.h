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

typedef struct alignas(16) : AlignedType
{
  // Filter #1 (Low band)

  Sample lf  ; // Frequency
  Sample f1p0; // Poles ...
  Sample f1p1;
  Sample f1p2;
  Sample f1p3;

  // Filter #2 (High band)

  Sample hf  ; // Frequency
  Sample f2p0; // Poles ...
  Sample f2p1;
  Sample f2p2;
  Sample f2p3;

  // Sample history buffer

  Sample sdm1; // Sample data minus 1
  Sample sdm2; // 2
  Sample sdm3; // 3

                    // Gain Controls

  Sample lg; // low gain
  Sample mg; // mid gain
  Sample hg; // high gain

  void init(double lowfreq, double mixfreq, double highfreq)
  {
    // Set Low/Mid/High gains to unity

    this->lg = Sample(1.0);
    this->mg = Sample(1.0);
    this->hg = Sample(1.0);

    // Calculate filter cutoff frequencies

    this->lf = Sample(2 * sin(3.14159265359 * ((double)lowfreq  / (double)mixfreq )));
    this->hf = Sample(2 * sin(3.14159265359 * ((double)highfreq / (double)mixfreq )));
  }

  Sample doThreeBand(Sample const& sample)
  {
    // Locals
    const Sample vsa = Sample(1.0 / 4294967295.0); // Very small amount (Denormal Fix)

    Sample l, m, h; // Low / Mid / High - Sample Values

                    // Filter #1 (lowpass)

    this->f1p0 += (this->lf * (sample     - this->f1p0)) + vsa;
    this->f1p1 += (this->lf * (this->f1p0 - this->f1p1));
    this->f1p2 += (this->lf * (this->f1p1 - this->f1p2));
    this->f1p3 += (this->lf * (this->f1p2 - this->f1p3));

    l = this->f1p3;

    // Filter #2 (highpass)

    this->f2p0 += (this->hf * (sample     - this->f2p0)) + vsa;
    this->f2p1 += (this->hf * (this->f2p0 - this->f2p1));
    this->f2p2 += (this->hf * (this->f2p1 - this->f2p2));
    this->f2p3 += (this->hf * (this->f2p2 - this->f2p3));

    h = this->sdm3 - this->f2p3;

    // Calculate midrange (signal - (low + high))

    m = this->sdm3 - (h + l);

    // Scale, Combine and store

    l *= this->lg;
    m *= this->mg;
    h *= this->hg;

    // Shuffle history buffer 

    this->sdm3 = this->sdm2;
    this->sdm2 = this->sdm1;
    this->sdm1 = sample;

    // Return result

    return(l + m + h);
  }

} Sample_EQSTATE;

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
  Sample_EQSTATE* es;
  const double vsa = (1.0 / 4294967295.0); // Very small amount (Denormal Fix)

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EqualizerAudioProcessor)
};

void processCompressor(Sample* data, size_t numberOfSamples, Sample_EQSTATE* es,
  float low, float mid, float high, float lowfreq, float highfreq);



#endif  // EQUALIZERAUDIOPROCESSOR_H_INCLUDED

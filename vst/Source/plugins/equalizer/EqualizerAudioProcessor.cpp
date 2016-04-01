/*
  ==============================================================================

    EqualizerAudioProcessor.cpp
    Created: 1 Apr 2016 8:04:43pm
    Author:  Dustin Kaiser

  ==============================================================================
*/


#ifdef PLUGIN_EQUALIZER

#include "EqualizerAudioProcessorEditor.h"
#include "EqualizerAudioProcessor.h"

EqualizerAudioProcessor::EqualizerAudioProcessor() 
{
  low  = new FloatParameter(0.f, "Low", 1.f, NormalizedRange(-12.f, 12.f));
  mid  = new FloatParameter(0.f, "Mid", 1.f, NormalizedRange(-12.f, 12.f));
  high = new FloatParameter(0.f, "High", 1.f, NormalizedRange(-12.f, 12.f));
  lowFreq = new FloatParameter(100.f, "LowFreq", 1.f, NormalizedRange(40.f, 20000.f));
  highFreq = new FloatParameter(5000.f, "HighFreq", 1.f, NormalizedRange(40.f, 20000.f));
  es = new EQSTATE();
  init_3band_state(100, 5000, 44100);

  addParameter(low);
  addParameter(mid);
  addParameter(high);
  addParameter(lowFreq);
  addParameter(highFreq);
}

void EqualizerAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
  // Use this method as the place to do any pre-playback
  // initialisation that you need..
}

void EqualizerAudioProcessor::releaseResources()
{
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

void EqualizerAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
  this->initializing(buffer);
  if (!this->getBypass())
  {
    double lowfreq = lowFreq->getValue();
    double highfreq = highFreq->getValue();
    double mixfreq = 44100;
    es->lf = 2 * sin(3.14159265359 * ((double)lowfreq / (double)mixfreq));
    es->hf = 2 * sin(3.14159265359 * ((double)highfreq / (double)mixfreq));
    es->lg = aux::decibelToLinear(low->getValue());
    es->mg = aux::decibelToLinear(mid->getValue());
    es->hg = aux::decibelToLinear(high->getValue());

    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
      for (int channel = 0; channel < getNumInputChannels(); channel++)
      {
        float* channelData = buffer.getWritePointer(channel);
        channelData[i] = do_3band(channelData[i]);
      }
    }
  }
  this->finalizing(buffer);
  this->meteringBuffer(buffer);
}

double EqualizerAudioProcessor::do_3band(double sample)
{
  // Locals

  double l, m, h; // Low / Mid / High - Sample Values

  // Filter #1 (lowpass)

  es->f1p0 += (es->lf * (sample - es->f1p0)) + vsa;
  es->f1p1 += (es->lf * (es->f1p0 - es->f1p1));
  es->f1p2 += (es->lf * (es->f1p1 - es->f1p2));
  es->f1p3 += (es->lf * (es->f1p2 - es->f1p3));

  l = es->f1p3;

  // Filter #2 (highpass)

  es->f2p0 += (es->hf * (sample - es->f2p0)) + vsa;
  es->f2p1 += (es->hf * (es->f2p0 - es->f2p1));
  es->f2p2 += (es->hf * (es->f2p1 - es->f2p2));
  es->f2p3 += (es->hf * (es->f2p2 - es->f2p3));

  h = es->sdm3 - es->f2p3;

  // Calculate midrange (signal - (low + high))

  m = es->sdm3 - (h + l);

  // Scale, Combine and store

  l *= es->lg;
  m *= es->mg;
  h *= es->hg;

  // Shuffle history buffer 

  es->sdm3 = es->sdm2;
  es->sdm2 = es->sdm1;
  es->sdm1 = sample;

  // Return result

  return(l + m + h);
}

void EqualizerAudioProcessor::init_3band_state(int lowfreq, int highfreq, int mixfreq)
{
  // Clear state 

  memset(es, 0, sizeof(EQSTATE));

  // Set Low/Mid/High gains to unity

  es->lg = 1.0;
  es->mg = 1.0;
  es->hg = 1.0;

  // Calculate filter cutoff frequencies

  es->lf = 2 * sin(3.14159265359 * ((double)lowfreq / (double)mixfreq));
  es->hf = 2 * sin(3.14159265359 * ((double)highfreq / (double)mixfreq));
}

AudioProcessorEditor* EqualizerAudioProcessor::createEditor()
{
  return new EqualizerAudioProcessorEditor(*this);
}

var EqualizerAudioProcessor::getState()
{
  DynamicObject* properties = new DynamicObject;
  properties->setProperty("low", low->getValue());
  properties->setProperty("mid", mid->getValue());
  properties->setProperty("high", high->getValue());
  properties->setProperty("lowFreq", lowFreq->getValue());
  properties->setProperty("highFreq", highFreq->getValue());
  return var(&properties);
}

void EqualizerAudioProcessor::setState(const var & state)
{
  low->setValue(state.getProperty("low", low->getValue()));
  mid->setValue(state.getProperty("mid", mid->getValue()));
  high->setValue(state.getProperty("high", high->getValue()));
  lowFreq->setValue(state.getProperty("lowFreq", lowFreq->getValue()));
  highFreq->setValue(state.getProperty("highFreq", highFreq->getValue()));
}

void EqualizerAudioProcessor::setLow(float value)
{
  low->setValue(value);
}

void EqualizerAudioProcessor::setMid(float value)
{
  mid->setValue(value);
}

void EqualizerAudioProcessor::setHigh(float value)
{
  high->setValue(value);
}

void EqualizerAudioProcessor::setLowFreq(float value)
{
  lowFreq->setValue(value);
}

void EqualizerAudioProcessor::setHighFreq(float value)
{
  highFreq->setValue(value);
}

float EqualizerAudioProcessor::getLow()
{
  return low->getValue();
}

float EqualizerAudioProcessor::getMid()
{
  return mid->getValue();
}

float EqualizerAudioProcessor::getHigh()
{
  return high->getValue();
}

float EqualizerAudioProcessor::getLowFreq()
{
  return lowFreq->getValue();
}

float EqualizerAudioProcessor::getHighFreq()
{
  return highFreq->getValue();
}

#endif
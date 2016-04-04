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
  es = new Sample_EQSTATE();
  es->init(100., 44100., 5000.);

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
    processBlockwise<FilterConstants::blockSize>(
      buffer, internalBuffer, [this](size_t samples, size_t offset)
    {
      processCompressor(internalBuffer, samples, es,
        getLow(), getMid(), getHigh(), getLowFreq(), getHighFreq());
    });
  }
  this->finalizing(buffer);
  this->meteringBuffer(buffer);
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
  return var(properties);
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
  low->beginChangeGesture();
  low->setUnnormalizedValue(value);
  low->endChangeGesture();
}

void EqualizerAudioProcessor::setMid(float value)
{
  mid->beginChangeGesture();
  mid->setUnnormalizedValue(value);
  mid->endChangeGesture();
}

void EqualizerAudioProcessor::setHigh(float value)
{
  high->beginChangeGesture();
  high->setUnnormalizedValue(value);
  high->endChangeGesture();
}

void EqualizerAudioProcessor::setLowFreq(float value)
{
  lowFreq->beginChangeGesture();
  lowFreq->setUnnormalizedValue(value);
  lowFreq->endChangeGesture();
}

void EqualizerAudioProcessor::setHighFreq(float value)
{
  highFreq->beginChangeGesture();
  highFreq->setUnnormalizedValue(value);
  highFreq->endChangeGesture();
}

float EqualizerAudioProcessor::getLow()
{
  return low->getUnnormalizedValue();
}

float EqualizerAudioProcessor::getMid()
{
  return mid->getUnnormalizedValue();
}

float EqualizerAudioProcessor::getHigh()
{
  return high->getUnnormalizedValue();
}

float EqualizerAudioProcessor::getLowFreq()
{
  return lowFreq->getUnnormalizedValue();
}

float EqualizerAudioProcessor::getHighFreq()
{
  return highFreq->getUnnormalizedValue();
}

void processCompressor(Sample* data, size_t numberOfSamples, Sample_EQSTATE* es,
  float low, float mid, float high, float lowfreq, float highfreq)
{
  const double mixfreq = 44100;
  es->lf = Sample(2 * sin(3.14159265359 * ((double)lowfreq  / mixfreq)));
  es->hf = Sample(2 * sin(3.14159265359 * ((double)highfreq / mixfreq)));
  es->lg = Sample(aux::decibelToLinear(low));
  es->mg = Sample(aux::decibelToLinear(mid));
  es->hg = Sample(aux::decibelToLinear(high));

  for (int i = 0; i < numberOfSamples; i++)
  {
      data[i] = es->doThreeBand(data[i]);
  }
}
#endif
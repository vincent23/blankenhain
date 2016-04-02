/*
  ==============================================================================

    DelayAudioProcessor.cpp
    Created: 1 Apr 2016 8:04:43pm
    Author:  Dustin Kaiser

  ==============================================================================
*/


#ifdef PLUGIN_DELAY

#include "DelayAudioProcessorEditor.h"
#include "DelayAudioProcessor.h"

DelayAudioProcessor::DelayAudioProcessor() : delayline(44100u)
{
  pan  = new FloatParameter(0.f, "pan", 1.f, NormalizedRange(-1.f, 1.f));
  length  = new FloatParameter(100.f, "length", 1.f, NormalizedRange(1.f, 5000.f));
  feedback = new FloatParameter(0.f, "feedback", 1.f, NormalizedRange(0.f, 1.f));
  drywet = new FloatParameter(0.f, "drywet", 1.f, NormalizedRange(0.f, 1.f));

  addParameter(pan);
  addParameter(length);
  addParameter(feedback);
  addParameter(drywet);
}

void DelayAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
  // Use this method as the place to do any pre-playback
  // initialisation that you need..
}

void DelayAudioProcessor::releaseResources()
{
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

void DelayAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
  this->initializing(buffer);
  if (this->getPlayHead() != nullptr)
  {
    AudioPlayHead::CurrentPositionInfo info;
    this->getPlayHead()->getCurrentPosition(info);
    this->bpm = info.bpm;
  }
  if (!this->getBypass())
  {
    delayline.setSize(aux::millisecToSamples(length->getValue()));
    float feedback_ = feedback->getValue();
    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
      float avg = 0.f;
      for (int channel = 0; channel < getNumInputChannels(); channel++)
      {
        float* channelData = buffer.getWritePointer(channel);
        avg += channelData[i];
      }
      avg /= static_cast<float>(getNumInputChannels());

      float original0 = buffer.getWritePointer(0)[i];
      float original1 = buffer.getWritePointer(1)[i];
      if (drywet->getValue() > 0.5)
      {
        original0 *= (1 - drywet->getValue()) * 2.f;
        original1 *= (1 - drywet->getValue()) * 2.f;
      }
      float line = delayline.get();
      if (drywet->getValue() < 0.5) line *= drywet->getValue() * 2.f;

      if(pan->getValue() <= 0.)
      {
        buffer.getWritePointer(0)[i] = original0 +  line;
        buffer.getWritePointer(1)[i] = original1 + (line * (1.f + pan->getValue()));
      }
      else if (pan->getValue() >= 0.)
      {
        buffer.getWritePointer(0)[i] = original0 + (line * (1.f - pan->getValue()));
        buffer.getWritePointer(1)[i] = original1 + line;
      }
      delayline.push((delayline.get()) * feedback->getValue() + avg);
    }
  }
  this->finalizing(buffer);
  this->meteringBuffer(buffer);
}

AudioProcessorEditor* DelayAudioProcessor::createEditor()
{
  return new DelayAudioProcessorEditor(*this);
}

var DelayAudioProcessor::getState()
{
  DynamicObject* properties = new DynamicObject;
  properties->setProperty("length", length->getValue());
  properties->setProperty("pan", pan->getValue());
  properties->setProperty("feedback", feedback->getValue());
  properties->setProperty("drywet", drywet->getValue());
  return var(&properties);
}

void DelayAudioProcessor::setState(const var & state)
{
  length->setValue(state.getProperty("length", length->getValue()));
  pan->setValue(state.getProperty("pan", pan->getValue()));
  feedback->setValue(state.getProperty("feedback", feedback->getValue()));
  drywet->setValue(state.getProperty("drywet", drywet->getValue()));
}

void DelayAudioProcessor::setLength(float value)
{
  length->setValueNotifyingHost(value);
}

void DelayAudioProcessor::setPan(float value)
{
  pan->setValueNotifyingHost(value);
}

void DelayAudioProcessor::setFeedback(float value)
{
  feedback->setValueNotifyingHost(value);
}

void DelayAudioProcessor::setDrywet(float value)
{
  drywet->setValueNotifyingHost(value);
}

float DelayAudioProcessor::getLength() const
{
  return length->getValue();
}

float DelayAudioProcessor::getDrywet() const
{
  return drywet->getValue();
}

float DelayAudioProcessor::getPan() const
{
  return pan->getValue();
}

float DelayAudioProcessor::getBPM() const
{
  return bpm;
}

float DelayAudioProcessor::getFeedback() const
{
  return feedback->getValue();
}

#endif
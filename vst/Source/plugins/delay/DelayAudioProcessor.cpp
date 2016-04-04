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
  feedback = new FloatParameter(0.f, "feedback", 1.f, NormalizedRange(0.f, 1.5f));
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
    this->bpm = static_cast<float>(info.bpm);
  }
  if (!this->getBypass())
  {
    processBlockwise<FilterConstants::blockSize>(
      buffer, internalBuffer, 
      [this](size_t samples, size_t offset)
      {
        processDelay(internalBuffer, samples,
          getPan(), getLength(), getFeedback(), getDrywet(),
          delayline);
      }
    );
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
  return var(properties);
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
  length->beginChangeGesture();
  length->setUnnormalizedValue(value);
  length->endChangeGesture();
}

void DelayAudioProcessor::setPan(float value)
{
  pan->beginChangeGesture();
  pan->setUnnormalizedValue(value);
  pan->endChangeGesture();
}

void DelayAudioProcessor::setFeedback(float value)
{
  feedback->beginChangeGesture();
  feedback->setUnnormalizedValue(value);
  feedback->endChangeGesture();
}

void DelayAudioProcessor::setDrywet(float value)
{
  drywet->beginChangeGesture();
  drywet->setUnnormalizedValue(value);
  drywet->endChangeGesture();
}

float DelayAudioProcessor::getLength() const
{
  return length->getUnnormalizedValue();
}

float DelayAudioProcessor::getDrywet() const
{
  return drywet->getUnnormalizedValue();
}

float DelayAudioProcessor::getPan() const
{
  return pan->getUnnormalizedValue();
}

float DelayAudioProcessor::getBPM() const
{
  return bpm;
}

float DelayAudioProcessor::getFeedback() const
{
  return feedback->getUnnormalizedValue();
}

// Pan between -1 and 1
void processDelay(Sample* data, size_t numberOfSamples,
  float pan, float length, float feedback, float drywet,
  CircularBuffer<Sample>& delayline)
{
  delayline.setSize(static_cast<size_t>(aux::millisecToSamples(length)));
  for (size_t i = 0; i < numberOfSamples; i++)
  {
    double avg_ = avg(data[i]);
    Sample original = data[i];
    Sample line = delayline.get();

    if (drywet > 0.5)
    {
      original *= Sample((1 - drywet) * 2.f);
    }
    else line *= Sample(drywet * 2.f);

    // Pan
    alignas(16) double lr[2];
    line.store_aligned(lr);
    lr[0] = lr[0] * (1.f - std::max(0.0f, pan));
    lr[1] = lr[1] * (1.f + std::min(0.0f, pan));
    line = load_aligned(lr);

    data[i] = original + line;

    delayline.push((delayline.get()) * Sample(feedback) + Sample(avg_));
  }
}

#endif
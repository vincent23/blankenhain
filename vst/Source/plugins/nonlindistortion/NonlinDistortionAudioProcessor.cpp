/*
  ==============================================================================

    NonlinDistortionAudioProcessor.cpp
    Created: 1 Apr 2016 8:04:43pm
    Author:  Dustin Kaiser

  ==============================================================================
*/


#ifdef PLUGIN_NONLIN_DISTORTION

#include "NonlinDistortionAudioProcessorEditor.h"
#include "NonlinDistortionAudioProcessor.h"

NonlinDistortionAudioProcessor::NonlinDistortionAudioProcessor()
{
  ingaindist = new FloatParameter(0., "inGain", 1., NormalizedRange(-12., 12.));
  iterations = new FloatParameter(1., "inGain", 1., NormalizedRange(1., 10.));
  addParameter(ingaindist);
  addParameter(iterations);
}

void NonlinDistortionAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
  // Use this method as the place to do any pre-playback
  // initialisation that you need..
}

void NonlinDistortionAudioProcessor::releaseResources()
{
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

void NonlinDistortionAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
  this->initializing(buffer);
  if (!this->getBypass())
  {
    processBlockwise<constants::blockSize>(
      buffer, internalBuffer, [this](size_t samples, size_t offset)
    {
      effects::distortion::processNonlinDistortion(internalBuffer, samples, getAlg(), getIngain(), getIterations());
    });
  }
  this->finalizing(buffer);
  this->meteringBuffer(buffer);
}

AudioProcessorEditor* NonlinDistortionAudioProcessor::createEditor()
{
  return new NonlinDistortionAudioProcessorEditor(*this);
}

var NonlinDistortionAudioProcessor::getState()
{
  DynamicObject* properties = new DynamicObject;
  properties->setProperty("alg", static_cast<int>(getAlg()));
  properties->setProperty("ingain", getIngain());
  properties->setProperty("iterations", static_cast<int>(getIterations()));
  return var(properties);
}

void NonlinDistortionAudioProcessor::setState(const var & state)
{
  setAlg(static_cast<int>( state.getProperty("alg", static_cast<int>(getAlg())) ));
  setIngain(state.getProperty("ingain", getIngain()));
  setIterations(static_cast<int>( state.getProperty("iterations", static_cast<int>(getIterations()))));
}

void NonlinDistortionAudioProcessor::setAlg(size_t value)
{
  if (value == 0) this->alg = Algorithm::none;
  else if (value == 1) this->alg = Algorithm::Araya_Suyama;
  else if (value == 2) this->alg = Algorithm::Doidic_sym;
  else if (value == 3) this->alg = Algorithm::Doidic_asym;
}

size_t NonlinDistortionAudioProcessor::getAlg() const
{
  if (this->alg == Algorithm::none) return 0u;
  else if (this->alg == Algorithm::Araya_Suyama) return 1u;
  else if (this->alg == Algorithm::Doidic_sym) return 2u;
  else if (this->alg == Algorithm::Doidic_asym) return 3u;
  else return 0u;
}

size_t NonlinDistortionAudioProcessor::getIterations() const
{
  return static_cast<size_t>(iterations->getUnnormalizedValue());
}

float NonlinDistortionAudioProcessor::getIngain() const
{
  return ingaindist->getUnnormalizedValue();
}
void NonlinDistortionAudioProcessor::setIterations(size_t in)
{
  iterations->beginChangeGesture();
  iterations->setUnnormalizedValue(static_cast<float>(in));
  iterations->endChangeGesture();
}
void NonlinDistortionAudioProcessor::setIngain(float in)
{
  ingaindist->beginChangeGesture();
  ingaindist->setUnnormalizedValue(in);
  ingaindist->endChangeGesture();
}


#endif
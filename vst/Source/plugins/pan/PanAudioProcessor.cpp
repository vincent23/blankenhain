#ifdef PLUGIN_PAN

#include "PanAudioProcessor.h"
#include "PanAudioProcessorEditor.h"

PanAudioProcessor::PanAudioProcessor()
{
	panning = new FloatParameter(0.0f, "panning", 0.5f, NormalizedRange(-50.f, 50.f));
  mono = new BoolParameter("mono", false);
  addParameter(panning);
  addParameter(mono);
}

void PanAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
}

void PanAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

void PanAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	this->initializing(buffer);
  if (!this->getBypass())
  {
    processBlockwise<constants::blockSize>(buffer, internalBuffer, [this](size_t samples, size_t offset) {
      effects::pan::processPanning(internalBuffer, samples, getPanning(), getMono());
    });
  }
	this->finalizing(buffer);
  this->meteringBuffer(buffer);
}

AudioProcessorEditor* PanAudioProcessor::createEditor()
{
	return new PanAudioProcessorEditor(*this);
}

void PanAudioProcessor::setPanning(float value) {
  panning->beginChangeGesture();
  panning->setUnnormalizedValue(value);
  panning->endChangeGesture();
}

void PanAudioProcessor::setMono(bool value)
{
  mono->setBoolValue(value);
}

var PanAudioProcessor::getState()
{
	DynamicObject* properties = new DynamicObject;
	properties->setProperty("panning", panning->getValue());
	return var(properties);
}

void PanAudioProcessor::setState(const var & state)
{
	panning->setValue(state.getProperty("panning", panning->getValue()));
}

float PanAudioProcessor::getPanning() {
	return panning->getUnnormalizedValue();
}

bool PanAudioProcessor::getMono()
{
  return mono->getBoolValue();
}

#endif
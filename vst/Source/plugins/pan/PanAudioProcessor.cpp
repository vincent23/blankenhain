#ifdef PLUGIN_PAN

#include "PanAudioProcessor.h"
#include "PanAudioProcessorEditor.h"

PanAudioProcessor::PanAudioProcessor()
{
	panning = new FloatParameter(0.0f, "panning", 0.5f, NormalizedRange(-50.f, 50.f));
	addParameter(panning);
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
    for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i) {
      buffer.clear(i, 0, buffer.getNumSamples());
    }
    // currentPanning: Set by Editor before this buffer iteration
    // oldPanning: Was Set in Editor after last buffer Iteration
    // Interpolation from oldPanning to currentPanning
    // momentaryPanning: Helper Variable, keeps results of current Interpolation iteration during Interpolation
    float currentPanning, oldPanning, bufferValue, momentaryPanning;

    unsigned int maxInterpolation;
    currentPanning = panning->getNormalizedValue();
    oldPanning = panning->getNormalizedOldValue();
    maxInterpolation = int(buffer.getNumSamples() * panning->getBufferScalingValue());

    if (getNumInputChannels() == 1)
    {
      for (size_t interpolationIteration = 0; interpolationIteration < maxInterpolation; interpolationIteration++)
      {
        bufferValue = buffer.getSample(/*channel*/ 0, interpolationIteration);
        momentaryPanning = (oldPanning + ((interpolationIteration + 1) * (currentPanning - oldPanning) / maxInterpolation));
        buffer.setSample(/*channel*/ 0, interpolationIteration, bufferValue * \
          (1.f - 2 * (std::max(0.5f, momentaryPanning) - 0.5f)));

        buffer.setSample(/*channel*/ 1, interpolationIteration, bufferValue * \
          2 * (std::min(0.5f, momentaryPanning)));
      }
      for (size_t bufferIteration = maxInterpolation; static_cast<int>(bufferIteration) < buffer.getNumSamples(); bufferIteration++)
      {
        bufferValue = buffer.getSample(/*channel*/ 0, bufferIteration);
        buffer.setSample(/*channel*/ 0, bufferIteration, bufferValue * (1.f - 2 * (std::max(0.5f, currentPanning) - 0.5f)));
        buffer.setSample(/*channel*/ 1, bufferIteration, bufferValue * 2 * (std::min(0.5f, currentPanning)));
      }
    }
    else
    {
      for (size_t interpolationIteration = 0; interpolationIteration < maxInterpolation; interpolationIteration++)
      {
        momentaryPanning = (oldPanning + ((interpolationIteration + 1) * (currentPanning - oldPanning) / maxInterpolation));

        bufferValue = buffer.getSample(/*channel*/ 0, interpolationIteration);
        buffer.setSample(/*channel*/ 0, interpolationIteration, bufferValue * \
          (1.f - 2 * (std::max(0.5f, momentaryPanning) - 0.5f)));
        bufferValue = buffer.getSample(/*channel*/ 1, interpolationIteration);
        buffer.setSample(/*channel*/ 1, interpolationIteration, bufferValue * \
          2 * (std::min(0.5f, momentaryPanning)));
      }
      for (size_t bufferIteration = maxInterpolation; static_cast<int>(bufferIteration) < buffer.getNumSamples(); bufferIteration++)
      {
        bufferValue = buffer.getSample(/*channel*/ 0, bufferIteration);
        buffer.setSample(/*channel*/ 0, bufferIteration, bufferValue * (1.f - 2 * (std::max(0.5f, currentPanning) - 0.5f)));
        bufferValue = buffer.getSample(/*channel*/ 1, bufferIteration);
        buffer.setSample(/*channel*/ 1, bufferIteration, bufferValue * 2 * (std::min(0.5f, currentPanning)));
      }
    }
    panning->setUnnormalizedOldValue();
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

#endif
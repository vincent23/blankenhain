#ifdef PLUGIN_VOLUME

#include "VolumeAudioProcessor.h"
#include "VolumeAudioProcessorEditor.h"

VolumeAudioProcessor::VolumeAudioProcessor()
{
	volumeL = new FloatParameter(120.f / 132.f, "volumeL", 0.5f, NormalizedRange(-120.f, 12.f, 5.f));
	volumeR = new FloatParameter(120.f / 132.f, "volumeR", 0.5f, NormalizedRange(-120.f, 12.f, 5.f));
	stereoCoupling = new BoolParameter("stereoCoupling", true);
	addParameter(volumeL);
	addParameter(volumeR);
	addParameter(stereoCoupling);
}

void VolumeAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
}

void VolumeAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

void VolumeAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	this->initializing(buffer);
  if (!this->getBypass())
  {
    float currentVolumeL, currentVolumeR, bufferValue, oldVolumeL, oldVolumeR;
    size_t maxInterpolation;

    if (stereoCoupling->getBoolValue())
    {
      currentVolumeL = aux::decibelToLinear(volumeL->getUnnormalizedValue());
      oldVolumeL = aux::decibelToLinear(volumeL->getUnnormalizedOldValue());
      maxInterpolation = int(buffer.getNumSamples() * volumeL->getBufferScalingValue());
      for (size_t interpolationIteration = 0; interpolationIteration < maxInterpolation; interpolationIteration++)
      {
        bufferValue = buffer.getSample(/*channel*/ 0, interpolationIteration);

        buffer.setSample(/*channel*/ 0, interpolationIteration, bufferValue * \
          (oldVolumeL + ((interpolationIteration + 1) * (currentVolumeL - oldVolumeL) \
            / maxInterpolation)));

        bufferValue = buffer.getSample(/*channel*/ 1, interpolationIteration);

        buffer.setSample(/*channel*/ 1, interpolationIteration, bufferValue * \
          (oldVolumeL + ((interpolationIteration + 1) * (currentVolumeL - oldVolumeL) \
            / maxInterpolation)));

      }
      for (
        size_t bufferIteration = maxInterpolation; 
        static_cast<int>(bufferIteration) < buffer.getNumSamples(); 
        bufferIteration++
        )
      {
        bufferValue = buffer.getSample(/*channel*/ 0, bufferIteration);
        buffer.setSample(/*channel*/ 0, bufferIteration, bufferValue * currentVolumeL);
        bufferValue = buffer.getSample(/*channel*/ 1, bufferIteration);
        buffer.setSample(/*channel*/ 1, bufferIteration, bufferValue * currentVolumeL);

      }
    }
    else
    {
      currentVolumeR = aux::decibelToLinear(volumeR->getUnnormalizedValue());
      currentVolumeL = aux::decibelToLinear(volumeL->getUnnormalizedValue());
      oldVolumeL = aux::decibelToLinear(volumeL->getUnnormalizedOldValue());
      oldVolumeR = aux::decibelToLinear(volumeR->getUnnormalizedOldValue());
      maxInterpolation = int(buffer.getNumSamples() * volumeL->getBufferScalingValue());
      for (size_t interpolationIteration = 0; interpolationIteration < maxInterpolation; interpolationIteration++)
      {
        bufferValue = buffer.getSample(/*channel*/ 0, interpolationIteration);

        buffer.setSample(/*channel*/ 0, interpolationIteration, bufferValue * \
          (oldVolumeL + ((interpolationIteration + 1) * (currentVolumeL - oldVolumeL) \
            / maxInterpolation)));

        bufferValue = buffer.getSample(/*channel*/ 1, interpolationIteration);

        buffer.setSample(/*channel*/ 1, interpolationIteration, bufferValue * \
          (oldVolumeR + ((interpolationIteration + 1) * (currentVolumeR - oldVolumeR) \
            / maxInterpolation)));


      }
      for (size_t bufferIteration = maxInterpolation; static_cast<int>(bufferIteration) < buffer.getNumSamples(); bufferIteration++)
      {
        bufferValue = buffer.getSample(/*channel*/ 0, bufferIteration);
        buffer.setSample(/*channel*/ 0, bufferIteration, bufferValue * currentVolumeL);
        bufferValue = buffer.getSample(/*channel*/ 1, bufferIteration);
        buffer.setSample(/*channel*/ 1, bufferIteration, bufferValue * currentVolumeR);
      }
    }
    //Set current values as old values for interpolation in next buffer iteration
    volumeL->setUnnormalizedOldValue(volumeL->getUnnormalizedValue());
    volumeR->setUnnormalizedOldValue(volumeR->getUnnormalizedValue());
  }
  this->finalizing(buffer);
	this->meteringBuffer(buffer);

}

AudioProcessorEditor* VolumeAudioProcessor::createEditor()
{
	return new VolumeAudioProcessorEditor(*this);
}

void VolumeAudioProcessor::setVolumeL(float value) {
  volumeL->beginChangeGesture();
  volumeL->setUnnormalizedValue(value);
  volumeL->endChangeGesture();
}

void VolumeAudioProcessor::setVolumeR(float value)
{
  volumeR->beginChangeGesture();
  volumeR->setUnnormalizedValue(value);
  volumeR->endChangeGesture();
}

void VolumeAudioProcessor::setStereoCoupling(bool newStereoCoupling) {
  stereoCoupling->beginChangeGesture();
	stereoCoupling->setValueNotifyingHost((float) newStereoCoupling);
  stereoCoupling->endChangeGesture();
}

void VolumeAudioProcessor::switchStereoCoupling(void) {
  stereoCoupling->beginChangeGesture();
	stereoCoupling->setValueNotifyingHost( (float) (! stereoCoupling->getBoolValue() ) );
  stereoCoupling->endChangeGesture();
}

var VolumeAudioProcessor::getState()
{
	DynamicObject* properties = new DynamicObject;
	properties->setProperty("volumeL", volumeL->getValue());
	properties->setProperty("volumeR", volumeR->getValue());
	properties->setProperty("stereoCoupling", stereoCoupling->getBoolValue());
	return var(properties);
}

void VolumeAudioProcessor::setState(const var & state)
{
	volumeL->setValue(state.getProperty("volumeL", volumeL->getDefaultValue()));
	volumeR->setValue(state.getProperty("volumeR", volumeR->getDefaultValue()));
	stereoCoupling->setBoolValue(state.getProperty("stereoCoupling", stereoCoupling->getDefaultValue()));
}

float VolumeAudioProcessor::getVolumeL() {
	return (*volumeL).getUnnormalizedValue();
}

float VolumeAudioProcessor::getVolumeR() {
	return volumeR->getUnnormalizedValue();
}

bool VolumeAudioProcessor::getStereoCoupling() {
	return stereoCoupling->getBoolValue();
}

#endif
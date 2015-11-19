/*
  ==============================================================================

    VolumeAudioProcessor.cpp
    Created: 2 Nov 2015 10:44:30am
    Author:  Dusti

  ==============================================================================
*/

#include "VolumeAudioProcessor.h"
#include "VolumeAudioProcessorEditor.h"

VolumeAudioProcessor::VolumeAudioProcessor()
{
	volumeL = new FloatParameter("volumeL", 120.f / 132.f, 0.5f, NormalisableRange<float>(-120.f, 12.f, 0.f, 5.f));
	volumeR = new FloatParameter("volumeR", 120.f / 132.f, 0.5f, NormalisableRange<float>(-120.f, 12.f, 0.f, 5.f));
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

	float currentVolumeL, currentVolumeR, bufferValue, oldVolumeL, oldVolumeR;
	unsigned int maxInterpolation;

	if (stereoCoupling->getBoolValue()) 
	{
		currentVolumeL = pow(10.f, (volumeL->getValue()) / 10.f);
		oldVolumeL = pow(10.f, (volumeL->getOldValue()) / 10.f);
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
		for (size_t bufferIteration = maxInterpolation; bufferIteration < buffer.getNumSamples(); bufferIteration++)
		{
			bufferValue = buffer.getSample(/*channel*/ 0, bufferIteration);
			buffer.setSample(/*channel*/ 0, bufferIteration, bufferValue * currentVolumeL);
			bufferValue = buffer.getSample(/*channel*/ 1, bufferIteration);
			buffer.setSample(/*channel*/ 1, bufferIteration, bufferValue * currentVolumeL);

		}
	}
	else
	{
		currentVolumeR = pow(10.f, volumeR->getValue() / 10.f);
		currentVolumeL = pow(10.f, volumeL->getValue() / 10.f);
		oldVolumeL = pow(10.f, volumeL->getOldValue() / 10.f);
		oldVolumeR = pow(10.f, volumeR->getOldValue() / 10.f);
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
			/ maxInterpolation )));


		}
		for (size_t bufferIteration = maxInterpolation; bufferIteration < buffer.getNumSamples(); bufferIteration++)
		{
			bufferValue = buffer.getSample(/*channel*/ 0, bufferIteration);
			buffer.setSample(/*channel*/ 0, bufferIteration, bufferValue * currentVolumeL);
			bufferValue = buffer.getSample(/*channel*/ 1, bufferIteration);
			buffer.setSample(/*channel*/ 1, bufferIteration, bufferValue * currentVolumeR);
		}
	}
	//Set current values as old values for interpolation in next buffer iteration
	volumeL->setOldValue(volumeL->getValue());
	volumeR->setOldValue(volumeR->getValue());

	this->meteringBuffer(buffer);
	this->finalizing(buffer);
}

AudioProcessorEditor* VolumeAudioProcessor::createEditor()
{
	return new VolumeAudioProcessorEditor(*this);
}

void VolumeAudioProcessor::setVolumeL(FloatParameter newVolumeL) {
	volumeL->setValueNotifyingHost(newVolumeL.getNormalizedValue());
}

void VolumeAudioProcessor::setVolumeR(FloatParameter newVolumeR) 
{
	volumeR->setValueNotifyingHost(newVolumeR.getNormalizedValue());
}

void VolumeAudioProcessor::setStereoCoupling(bool newStereoCoupling) {
	stereoCoupling->setValueNotifyingHost((float) newStereoCoupling);
}

void VolumeAudioProcessor::switchStereoCoupling(void) {
	stereoCoupling->setValueNotifyingHost( (float) (! stereoCoupling->getBoolValue() ) );
}

var VolumeAudioProcessor::getState()
{
	DynamicObject* properties = new DynamicObject;
	properties->setProperty("volumeL", volumeL->getValue());
	properties->setProperty("volumeR", volumeR->getValue());
	properties->setProperty("stereoCoupling", stereoCoupling->getBoolValue());
	return var(&properties);
}

void VolumeAudioProcessor::setState(const var & state)
{
	volumeL->setValue(state.getProperty("volumeL", volumeL->getDefaultValue()));
	volumeR->setValue(state.getProperty("volumeR", volumeR->getDefaultValue()));
	stereoCoupling->setBoolValue(state.getProperty("stereoCoupling", stereoCoupling->getDefaultValue()));
}

float VolumeAudioProcessor::getVolumeL() {
	return (*volumeL).getValue();
}

float VolumeAudioProcessor::getVolumeR() {
	return volumeR->getValue();
}

bool VolumeAudioProcessor::getStereoCoupling() {
	return stereoCoupling->getBoolValue();
}
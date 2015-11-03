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
	volumeL = new FloatParameter("volumeL", 120.f / 132.f);
	volumeR = new FloatParameter("volumeR", 120.f / 132.f);
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
	// In case we have more outputs than inputs, this code clears any output
	// channels that didn't contain input data, (because these aren't
	// guaranteed to be empty - they may contain garbage).
	// I've added this to avoid people getting screaming feedback
	// when they first compile the plugin, but obviously you don't need to
	// this code if your algorithm already fills all the output channels.
	for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i) {
		buffer.clear(i, 0, buffer.getNumSamples());
	}

	float currentVolumeL, currentVolumeR, bufferValue, oldVolumeL, oldVolumeR;

	if (stereoCoupling->getBoolValue()) 
	{
		currentVolumeL = pow(10.f, (volumeL->getValue() * 132.f - 120.f) / 10.f);
		oldVolumeL = pow(10.f, (volumeL->getOldValue() * 132.f - 120.f) / 10.f);
		for (size_t bufferIteration = 0; bufferIteration < buffer.getNumSamples(); bufferIteration++)
		{
			bufferValue = buffer.getSample(/*channel*/ 0, bufferIteration);
			buffer.setSample(/*channel*/ 0, bufferIteration, bufferValue * (oldVolumeL + ( (bufferIteration + 1) * (currentVolumeL - oldVolumeL) / buffer.getNumSamples() ) ));
			bufferValue = buffer.getSample(/*channel*/ 1, bufferIteration);
			buffer.setSample(/*channel*/ 1, bufferIteration, bufferValue * (oldVolumeL + ( (bufferIteration + 1) * (currentVolumeL - oldVolumeL) / buffer.getNumSamples() ) ));
		}
		volumeL->setOldValue(volumeL->getValue());
	}
	else
	{
		currentVolumeR = pow(10.f, (volumeR->getValue() * 132.f - 120.f) / 10.f);
		currentVolumeL = pow(10.f, (volumeL->getValue() * 132.f - 120.f) / 10.f);
		oldVolumeL = pow(10.f, (volumeL->getOldValue() * 132.f - 120.f) / 10.f);
		oldVolumeR = pow(10.f, (volumeR->getOldValue() * 132.f - 120.f) / 10.f);
		for (size_t bufferIteration = 0; bufferIteration < buffer.getNumSamples(); bufferIteration++)
		{
			bufferValue = buffer.getSample(/*channel*/ 0, bufferIteration);
			buffer.setSample(/*channel*/ 0, bufferIteration, bufferValue * (oldVolumeL + ((bufferIteration + 1) * (currentVolumeL - oldVolumeL) / buffer.getNumSamples())));
			bufferValue = buffer.getSample(/*channel*/ 1, bufferIteration);
			buffer.setSample(/*channel*/ 1, bufferIteration, bufferValue * (oldVolumeR + ((bufferIteration + 1) * (currentVolumeR - oldVolumeR) / buffer.getNumSamples())));
		}
		volumeL->setOldValue(volumeL->getValue());
		volumeR->setOldValue(volumeR->getValue());
	}
}

AudioProcessorEditor* VolumeAudioProcessor::createEditor()
{
	return new VolumeAudioProcessorEditor(*this);
}

void VolumeAudioProcessor::setVolumeL(float newVolumeL) {
	volumeL->setValueNotifyingHost((newVolumeL + 120.f) / 132.f);
}

/**
 * This set function will change the SLIDER value to a NORMALIZED
 * value so you do not have to worry :)
 */
void VolumeAudioProcessor::setVolumeR(float newVolumeR) {
	volumeR->setValueNotifyingHost((newVolumeR + 120.f) / 132.f);
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

/**
 * This get function will change the NORMALIZED value to a SLIDER
 * value so you do not have to worry :)
 */
float VolumeAudioProcessor::getVolumeL() {
	return (*volumeL).getValue() * 132.f - 120.f;
}

/**
 * This get function will change the NORMALIZED value to a SLIDER
 * value so you do not have to worry :)
 */
float VolumeAudioProcessor::getVolumeR() {
	return volumeR->getValue() * 132.f - 120.f;
}

bool VolumeAudioProcessor::getStereoCoupling() {
	return stereoCoupling->getBoolValue();
}
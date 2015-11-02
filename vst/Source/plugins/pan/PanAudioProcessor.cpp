/*
  ==============================================================================

    PanAudioProcessor.cpp
    Created: 2 Nov 2015 9:24:36pm
    Author:  Dusti

  ==============================================================================
*/

#include "PanAudioProcessor.h"
#include "PanAudioProcessorEditor.h"

PanAudioProcessor::PanAudioProcessor()
{
	panning = new FloatParameter("panning", (0.f + 50.f) / 100.f);
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
	// In case we have more outputs than inputs, this code clears any output
	// channels that didn't contain input data, (because these aren't
	// guaranteed to be empty - they may contain garbage).
	// I've added this to avoid people getting screaming feedback
	// when they first compile the plugin, but obviously you don't need to
	// this code if your algorithm already fills all the output channels.
	for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i) {
		buffer.clear(i, 0, buffer.getNumSamples());
	}

	float currentPanning, bufferValue;
	currentPanning = panning->getValue();
	if (getNumInputChannels() == 1) 
	{
		for (size_t bufferIteration = 0; bufferIteration < buffer.getNumSamples(); bufferIteration++)
		{
			bufferValue = buffer.getSample(/*channel*/ 0, bufferIteration);
			buffer.setSample(/*channel*/ 0, bufferIteration, bufferValue * (1.f - 2 * (std::max(0.5f, currentPanning) - 0.5f)));
			buffer.setSample(/*channel*/ 1, bufferIteration, bufferValue * 2 * (std::min(0.5f, currentPanning)));
		}
	}
	else
	{
		for (size_t bufferIteration = 0; bufferIteration < buffer.getNumSamples(); bufferIteration++)
		{
			bufferValue = buffer.getSample(/*channel*/ 0, bufferIteration);
			buffer.setSample(/*channel*/ 0, bufferIteration, bufferValue * (1.f - 2 * (std::max(0.5f, currentPanning) - 0.5f)));
			bufferValue = buffer.getSample(/*channel*/ 1, bufferIteration);
			buffer.setSample(/*channel*/ 1, bufferIteration, bufferValue * 2 * (std::min(0.5f, currentPanning) ) );
		}
	}
}

AudioProcessorEditor* PanAudioProcessor::createEditor()
{
	return new PanAudioProcessorEditor(*this);
}
/* THIS WILL DO CONVERSION TO NORMALIZED FLOAT LOLL SO SLEEek*/
void PanAudioProcessor::setPanning(float newPanning) {
	panning->setValueNotifyingHost((newPanning + 50.f) / 100.f);
}


var PanAudioProcessor::getState()
{
	DynamicObject* properties = new DynamicObject;
	properties->setProperty("panning", panning->getValue());
	return var(&properties);
}

void PanAudioProcessor::setState(const var & state)
{
	panning->setValue(state.getProperty("panning", panning->getDefaultValue()));
}

float PanAudioProcessor::getPanning() {
	return (*panning).getValue() * 100.f - 50.f;
}

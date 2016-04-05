#ifdef PLUGIN_SAMPLE_PLAYER

#include "SamplePlayerAudioProcessor.h"
#include "SamplePlayerAudioProcessorEditor.h"

SamplePlayerAudioProcessor::SamplePlayerAudioProcessor()
{
}

void SamplePlayerAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
}

void SamplePlayerAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

void SamplePlayerAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	this->initializing(buffer);


	this->meteringBuffer(buffer);
	this->finalizing(buffer);
}

AudioProcessorEditor* SamplePlayerAudioProcessor::createEditor()
{
	return new SamplePlayerAudioProcessorEditor(*this);
}

var SamplePlayerAudioProcessor::getState()
{
	DynamicObject* properties = new DynamicObject;
	return var(properties);
}

void SamplePlayerAudioProcessor::setState(const var & state)
{
}

#endif
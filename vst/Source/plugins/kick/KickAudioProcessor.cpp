#ifdef PLUGIN_KICK

#include "KickAudioProcessor.h"
#include "KickAudioProcessorEditor.h"

KickAudioProcessor::KickAudioProcessor()
{
}

void KickAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
}

void KickAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

void KickAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	this->initializing(buffer);


	this->meteringBuffer(buffer);
	this->finalizing(buffer);
}

AudioProcessorEditor* KickAudioProcessor::createEditor()
{
	return new KickAudioProcessorEditor(*this);
}

var KickAudioProcessor::getState()
{
	DynamicObject* properties = new DynamicObject;
	return var(&properties);
}

void KickAudioProcessor::setState(const var & state)
{
}

#endif
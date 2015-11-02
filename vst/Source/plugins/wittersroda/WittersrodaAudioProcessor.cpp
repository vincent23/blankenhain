#include "WittersrodaAudioProcessor.h"
#include "WittersrodaAudioProcessorEditor.h"

WittersrodaAudioProcessor::WittersrodaAudioProcessor()
{
}

void WittersrodaAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void WittersrodaAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void WittersrodaAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
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

	// This is the place where you'd normally do the guts of your plugin's
	// audio processing...
	for (int channel = 0; channel < getNumInputChannels(); channel++) {
		float* channelData = buffer.getWritePointer (channel);

		// ..do something to the data...
	}
}

AudioProcessorEditor* WittersrodaAudioProcessor::createEditor()
{
	return new WittersrodaAudioProcessorEditor(*this);
}

var WittersrodaAudioProcessor::getState()
{
	return var();
}

void WittersrodaAudioProcessor::setState(const var & state)
{
}

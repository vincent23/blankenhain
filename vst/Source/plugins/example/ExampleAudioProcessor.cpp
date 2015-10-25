#include "ExampleAudioProcessor.h"
#include "ExampleAudioProcessorEditor.h"

ExampleAudioProcessor::ExampleAudioProcessor()
{
}

void ExampleAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void ExampleAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void ExampleAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
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

AudioProcessorEditor* ExampleAudioProcessor::createEditor()
{
	return new ExampleAudioProcessorEditor(*this);
}

var ExampleAudioProcessor::getState()
{
	return var();
}

void ExampleAudioProcessor::setState(const var & state)
{
}

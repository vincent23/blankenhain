#include "ExampleAudioProcessor.h"
#include "ExampleAudioProcessorEditor.h"

ExampleAudioProcessor::ExampleAudioProcessor()
{
}

ExampleAudioProcessor::~ExampleAudioProcessor()
{
}

const String ExampleAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

const String ExampleAudioProcessor::getInputChannelName(int channelIndex) const
{
    return String(channelIndex + 1);
}

const String ExampleAudioProcessor::getOutputChannelName(int channelIndex) const
{
    return String(channelIndex + 1);
}

bool ExampleAudioProcessor::isInputChannelStereoPair(int index) const
{
    return true;
}

bool ExampleAudioProcessor::isOutputChannelStereoPair(int index) const
{
    return true;
}

bool ExampleAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
    return false;
#endif
}

bool ExampleAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool ExampleAudioProcessor::silenceInProducesSilenceOut() const
{
	return false;
}

double ExampleAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int ExampleAudioProcessor::getNumPrograms()
{
	// NB: some hosts don't cope very well if you tell them there are 0 programs,
	// so this should be at least 1, even if you're not really implementing programs.
	return 1;
}

int ExampleAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ExampleAudioProcessor::setCurrentProgram (int index)
{
}

const String ExampleAudioProcessor::getProgramName (int index)
{
    return String();
}

void ExampleAudioProcessor::changeProgramName (int index, const String& newName)
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

bool ExampleAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ExampleAudioProcessor::createEditor()
{
	return new ExampleAudioProcessorEditor(*this);
}

void ExampleAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void ExampleAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

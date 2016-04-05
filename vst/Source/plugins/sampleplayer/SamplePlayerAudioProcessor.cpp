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

	MidiBuffer::Iterator midiIterator(midiMessages);
	MidiMessage message;
	int messagePosition;
	if (midiIterator.getNextEvent(message, messagePosition)) {
		{
			if (message.isNoteOn()) {
				position = 0;
			}
		}
	}

	if (sampleBuffer.getNumSamples() > 0 && position < sampleBuffer.getNumSamples()) {
		int remaining = sampleBuffer.getNumSamples() - position;
		int numberOfSamples = std::min(remaining, buffer.getNumSamples());
		buffer.copyFrom(0, 0, sampleBuffer, 0, position, numberOfSamples);
		buffer.copyFrom(1, 0, sampleBuffer, 1, position, numberOfSamples);
		position += numberOfSamples;
	}

	this->meteringBuffer(buffer);
	this->finalizing(buffer);
}

AudioBuffer<float>& SamplePlayerAudioProcessor::getSampleBuffer()
{
	return sampleBuffer;
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
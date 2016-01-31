#ifdef PLUGIN_COMPRESSOR

#include "CompressorAudioProcessor.h"
#include "CompressorAudioProcessorEditor.h"

CompressorAudioProcessor::CompressorAudioProcessor()
{
}

void CompressorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void CompressorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void CompressorAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
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
	jassert(getNumInputChannels() == 2);
	float attack = 0.2;
	float release = 1;
	float attackGain = exp(-1/(attack * 44100));
	float releaseGain = exp(-1 / (release * 44100));
	float threshold = -12;
	float ratio = 4;
	float slope = 1 - (1/ratio);
	for (int i = 0; i < buffer.getNumSamples(); i++) {
		for (int channel = 0; channel < getNumInputChannels(); channel++) {
			float* channelData = buffer.getWritePointer(channel);
			float input = abs(channelData[i]);
			if (envelope[channel] < input) {
				envelope[channel] = input + attackGain * (envelope[channel] - input);
			}
			else {
				envelope[channel] = input + releaseGain * (envelope[channel] - input);
			}
		}
		float envelopeValue = std::max(envelope[0], envelope[1]);
		for (int channel = 0; channel < getNumInputChannels(); channel++) {
			float* channelData = buffer.getWritePointer(channel);
			float input = channelData[i];
			float inputDb = 20 * log10(abs(input));
			float gainDb = std::min(0.f, slope * (threshold - inputDb));
			channelData[i] *= pow(10.f, gainDb / 20.f);
		}
	}
}

AudioProcessorEditor* CompressorAudioProcessor::createEditor()
{
	return new CompressorAudioProcessorEditor(*this);
}

var CompressorAudioProcessor::getState()
{
	return var();
}

void CompressorAudioProcessor::setState(const var & state)
{
}

#endif
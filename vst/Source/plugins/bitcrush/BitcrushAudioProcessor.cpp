#ifdef PLUGIN_BITCRUSH

#include "BitcrushAudioProcessor.h"
#include "BitcrushAudioProcessorEditor.h"

BitcrushAudioProcessor::BitcrushAudioProcessor()
{
	bitcrush = new FloatParameter(.5, "Bitcrush");
	downsample = new FloatParameter(.0, "Downsample");
	wet = new FloatParameter(1., "Dry/Wet");
	addParameter(bitcrush);
	addParameter(downsample);
	addParameter(wet);
}

void BitcrushAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void BitcrushAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void BitcrushAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	this->initializing(buffer);

	for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i) {
		buffer.clear(i, 0, buffer.getNumSamples());
	}

	float crush = bitcrush->getValue();
	float wet_ = wet->getValue();
	int groupedSamples = std::max(1.f, downsample->getValue() * 100);
	float bitdepth = 12. * (1. - crush) + 1. * crush;
	int steps = exp2(bitdepth);

	// This is the place where you'd normally do the guts of your plugin's
	// audio processing...
	for (int channel = 0; channel < getNumInputChannels(); channel++) 
	{
		for (int sample = 0; sample < buffer.getNumSamples() - groupedSamples; sample += groupedSamples) {
			float averagedSample = 0.;
			for (int i = 0; i < groupedSamples; i++) {
				averagedSample += buffer.getSample(channel, i + sample) / groupedSamples;
			}

			int discretizedSample = averagedSample * steps;
			float crushed = float(discretizedSample) / steps;

			for (int i = 0; i < groupedSamples; i++) {
				float sampleValue = buffer.getSample(channel, i + sample);
				buffer.setSample(channel, i + sample, sampleValue * (1. - wet_) + crushed * wet_);
			}
		}

		float averagedSample = 0.;
		for (int i = (buffer.getNumSamples() / groupedSamples) * groupedSamples; i < buffer.getNumSamples(); i++) {
			averagedSample += buffer.getSample(channel, i) / (buffer.getNumSamples() % groupedSamples);
		}

		float bitdepth = 12. * (1. - crush) + 1. * crush;
		int steps = exp2(bitdepth);
		int discretizedSample = averagedSample * steps;
		float crushed = float(discretizedSample) / steps;

		for (int i = (buffer.getNumSamples() / groupedSamples) * groupedSamples; i < buffer.getNumSamples(); i++) {
			float sampleValue = buffer.getSample(channel, i);
			buffer.setSample(channel, i, sampleValue * (1. - wet_) + crushed * wet_);
		}
	}

	this->meteringBuffer(buffer);
	this->finalizing(buffer);
}

AudioProcessorEditor* BitcrushAudioProcessor::createEditor()
{
	return new BitcrushAudioProcessorEditor(*this);
}

var BitcrushAudioProcessor::getState()
{
	DynamicObject* properties = new DynamicObject;
	properties->setProperty("bitcrush", bitcrush->getValue());
	properties->setProperty("downsample", downsample->getValue());
	properties->setProperty("wet", wet->getValue());
	return var(properties);
}

void BitcrushAudioProcessor::setState(const var & state)
{
	bitcrush->setValue(state.getProperty("bitcrush", bitcrush->getDefaultValue()));
	downsample->setValue(state.getProperty("downsample", downsample->getDefaultValue()));
	wet->setValue(state.getProperty("wet", wet->getDefaultValue()));
}

void BitcrushAudioProcessor::setBitcrush(float bitcrush_)
{
	bitcrush->setValueNotifyingHost(bitcrush_);
}

void BitcrushAudioProcessor::setDownsample(float downsample_)
{
	downsample->setValueNotifyingHost(downsample_);
}

void BitcrushAudioProcessor::setWet(float wet_)
{
	wet->setValueNotifyingHost(wet_);
}

float BitcrushAudioProcessor::getBitcrush() const
{
	return bitcrush->getValue();
}

float BitcrushAudioProcessor::getDownsample() const
{
	return downsample->getValue();
}

float BitcrushAudioProcessor::getWet() const
{
	return wet->getValue();
}

#endif
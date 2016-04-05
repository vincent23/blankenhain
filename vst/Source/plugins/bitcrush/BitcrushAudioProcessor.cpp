#ifdef PLUGIN_BITCRUSH

#include "BitcrushAudioProcessor.h"
#include "BitcrushAudioProcessorEditor.h"

BitcrushAudioProcessor::BitcrushAudioProcessor()
{
	bitcrush = new FloatParameter(.5, "Bitcrush", 1.f, NormalizedRange());
	downsample = new FloatParameter(.0, "Downsample", 1.f, NormalizedRange());
	wet = new FloatParameter(1., "Dry/Wet", 1.f, NormalizedRange());
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

	for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i) {
		buffer.clear(i, 0, buffer.getNumSamples());
	}
  if (!this->getBypass())
  {
    processBlockwise<constants::blockSize>(buffer, internalBuffer, [this](size_t samples, size_t offset) {
      effects::bitcrush::processBitcrush(internalBuffer, samples, getBitcrush(), getDownsample(), getWet());
    });
  }
  this->finalizing(buffer);
	this->meteringBuffer(buffer);

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

void BitcrushAudioProcessor::setBitcrush(float value)
{
  bitcrush->beginChangeGesture();
  bitcrush->setUnnormalizedValue(value);
  bitcrush->endChangeGesture();
}

void BitcrushAudioProcessor::setDownsample(float value)
{
  downsample->beginChangeGesture();
  downsample->setUnnormalizedValue(value);
  downsample->endChangeGesture();
}

void BitcrushAudioProcessor::setWet(float value)
{
  wet->beginChangeGesture();
  wet->setUnnormalizedValue(value);
  wet->endChangeGesture();
}

float BitcrushAudioProcessor::getBitcrush() const
{
	return bitcrush->getUnnormalizedValue();
}

float BitcrushAudioProcessor::getDownsample() const
{
	return downsample->getUnnormalizedValue();
}

float BitcrushAudioProcessor::getWet() const
{
  return wet->getUnnormalizedValue();
}



#endif
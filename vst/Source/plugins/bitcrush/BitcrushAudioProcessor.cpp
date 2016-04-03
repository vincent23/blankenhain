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
    float crush = bitcrush->getUnnormalizedValue();
    float wet_ = wet->getUnnormalizedValue();
    int groupedSamples = static_cast<int>(std::max(1.f, downsample->getUnnormalizedValue() * 100.f));
    float bitdepth = 12.f * (1.f - crush) + 1.f * crush;
    int steps = static_cast<int>(exp2(bitdepth));

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    for (int channel = 0; channel < getTotalNumInputChannels(); channel++)
    {
      for (int sample = 0; sample < buffer.getNumSamples() - groupedSamples; sample += groupedSamples) {
        float averagedSample = 0.;
        for (int i = 0; i < groupedSamples; i++) {
          averagedSample += buffer.getSample(channel, i + sample) / groupedSamples;
        }

        int discretizedSample = static_cast<int>(averagedSample) * steps;
        float crushed = float(discretizedSample) / steps;

        for (int i = 0; i < groupedSamples; i++) {
          float sampleValue = buffer.getSample(channel, i + sample);
          buffer.setSample(channel, i + sample, sampleValue * (1.f - wet_) + crushed * wet_);
        }
      }

      float averagedSample = 0.;
      for (int i = (buffer.getNumSamples() / groupedSamples) * groupedSamples; i < buffer.getNumSamples(); i++) {
        averagedSample += buffer.getSample(channel, i) / (buffer.getNumSamples() % groupedSamples);
      }

      int discretizedSample = static_cast<int>(averagedSample) * steps;
      float crushed = float(discretizedSample) / static_cast<float>(steps);

      for (int i = (buffer.getNumSamples() / groupedSamples) * groupedSamples; i < buffer.getNumSamples(); i++) {
        float sampleValue = buffer.getSample(channel, i);
        buffer.setSample(channel, i, sampleValue * (1.f - wet_) + crushed * wet_);
      }
    }
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
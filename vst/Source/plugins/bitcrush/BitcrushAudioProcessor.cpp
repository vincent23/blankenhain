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
    processBlockwise<FilterConstants::blockSize>(buffer, internalBuffer, [this](size_t samples, size_t offset) {
      processBitcrush(internalBuffer, samples, getBitcrush(), getDownsample(), getWet());
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

// Casts value of sample to int (discretization)
// and back to double
Sample discretize(Sample const& sample)
{
  alignas(16) double lr[2];
  sample.store_aligned(lr);
  lr[0] = static_cast<double>(static_cast<int>(lr[0]));
  lr[1] = static_cast<double>(static_cast<int>(lr[1]));
  return load_aligned(lr);
}

// Channel number is expected to be == 2
// All parameter values between 0 and 1
void processBitcrush(Sample* data, size_t numberOfSamples, float crush, float downsample,
  float wet)
{
  int groupedSamples = static_cast<int>(std::max(1.f, downsample * 100.f));
  float bitdepth = 12.f * (1.f - crush) + 1.f * crush;
  int steps = static_cast<int>(exp2(bitdepth));

  for (size_t sample = 0; sample < numberOfSamples - groupedSamples; sample += groupedSamples)
  {
    Sample averagedSample(0., 0.);
    for (int i = 0; i < groupedSamples; i++) 
    {
      averagedSample += data[i + sample] / Sample((double)groupedSamples);
    }

    Sample discretizedSample = discretize(averagedSample * Sample(static_cast<double>(steps)));
    discretizedSample /= Sample(static_cast<double>(steps));

    for (int i = 0; i < groupedSamples; i++) {
      Sample sampleValue = data[i + sample];
      data[i + sample] =  sampleValue * Sample((1. - wet) + crush * wet);
    }
  }

  Sample averagedSample(0., 0.);
  for (size_t i = (numberOfSamples / groupedSamples) * groupedSamples; i < numberOfSamples; i++)
  {
    averagedSample += data[i] / Sample(static_cast<double>((numberOfSamples % groupedSamples)));
  }

  Sample discretizedSample = discretize(averagedSample * Sample(static_cast<double>(steps)));
  discretizedSample /= Sample(static_cast<double>(steps));

  for (size_t i = (numberOfSamples / groupedSamples) * groupedSamples; i < numberOfSamples; i++)
  {
    Sample sampleValue = data[i];
    data[i] = sampleValue * Sample((1. - wet) + crush * wet);
  }
}

#endif
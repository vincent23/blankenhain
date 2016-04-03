#ifdef PLUGIN_FILTER

#include "FilterAudioProcessor.h"
#include "FilterAudioProcessorEditor.h"

FilterAudioProcessor::FilterAudioProcessor() :
	filterType(FilterType::Low)
{
	resonance = new FloatParameter(0.5, "Resonance", 0.5f, NormalizedRange(0.f, 5.f));
	frequency = new FloatParameter((500.f), "Frequency", 0.5f, NormalizedRange(40.f, 22000.f, 0.3f));

	addParameter(resonance);
	addParameter(frequency);
}

void FilterAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
}

void FilterAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

void FilterAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	this->initializing(buffer);
  if (!this->getBypass())
  {
    processBlockwise<FilterConstants::blockSize>(buffer, internalBuffer, [this](size_t samples, size_t offset) {
      switch (filterType) {
      case High:
        filter->processHigh(internalBuffer, samples, getFrequency(), getResonance());
        break;
      case Low:
        filter->processLow(internalBuffer, samples, getFrequency(), getResonance());
        break;
      case Band:
        filter->processBand(internalBuffer, samples, getFrequency(), getResonance());
        break;
      case Notch:
        filter->processNotch(internalBuffer, samples, getFrequency(), getResonance());
        break;
      }
    });
  }
  this->finalizing(buffer);
	this->meteringBuffer(buffer);
}

AudioProcessorEditor* FilterAudioProcessor::createEditor()
{
	return new FilterAudioProcessorEditor(*this);
}

void FilterAudioProcessor::setResonance(float value) {
  resonance->beginChangeGesture();
  resonance->setUnnormalizedValue(value);
  resonance->endChangeGesture();
}

void FilterAudioProcessor::setFrequency(float value) {
  frequency->beginChangeGesture();
  frequency->setUnnormalizedValue(value);
  frequency->endChangeGesture();
}

void FilterAudioProcessor::setFilterType(int index) {
	switch (index) {
	case 0:
		filterType = FilterType::High;
		break;
	case 1:
		filterType = FilterType::Low;
		break;
	case 2:
		filterType = FilterType::Band;
		break;
	case 3:
		filterType = FilterType::Notch;
		break;
	}
}

var FilterAudioProcessor::getState()
{
	DynamicObject* properties = new DynamicObject;
	properties->setProperty("frequency", frequency->getValue());
	properties->setProperty("resonance", resonance->getValue());
	properties->setProperty("filterType", filterType);
	return var(properties);
}

void FilterAudioProcessor::setState(const var & state)
{
	frequency->setValue(state.getProperty("frequency", frequency->getDefaultValue()));
	resonance->setValue(state.getProperty("resonance", resonance->getDefaultValue()));
	setFilterType(state.getProperty("filterType", 0));
}

float FilterAudioProcessor::getResonance() {
	return resonance->getUnnormalizedValue();
}

float FilterAudioProcessor::getFrequency() {
	return frequency->getUnnormalizedValue();
}

int FilterAudioProcessor::getFilterType() {
	return filterType;
}

#endif
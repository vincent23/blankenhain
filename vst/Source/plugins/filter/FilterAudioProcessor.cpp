#include "FilterAudioProcessor.h"
#include "FilterAudioProcessorEditor.h"

FilterAudioProcessor::FilterAudioProcessor() :
	filterType(FilterType::Low)
{
	resonance = new FloatParameter(0.5, "Resonance", 0.5f, juce::NormalisableRange<float>(0.f, 5.f));
	frequency = new FloatParameter((500.f), "Frequency", 0.5f, juce::NormalisableRange<float>(40.f, 22000.f,0.f,0.3f));

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
	// In case we have more outputs than inputs, this code clears any output
	// channels that didn't contain input data, (because these aren't
	// guaranteed to be empty - they may contain garbage).
	// I've added this to avoid people getting screaming feedback
	// when they first compile the plugin, but obviously you don't need to
	// this code if your algorithm already fills all the output channels.
	for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i) {
		buffer.clear(i, 0, buffer.getNumSamples());
	}
	//internalBuffer = new Sample[bufferSize];
	// This is the place where you'd normally do the guts of your plugin's
	// audio processing...

	const size_t bufferIterationCount = buffer.getNumSamples() / bufferSize;
	for (size_t bufferIteration = 0; bufferIteration < bufferIterationCount; bufferIteration++) {
		for (size_t i = 0; i < bufferSize; i++) {
			int sampleIndex = bufferIteration * bufferSize + i;
			internalBuffer[i] = Sample(buffer.getSample(0, sampleIndex), buffer.getSample(1, sampleIndex));
		}
		switch (filterType) {
		case High:
			filter->processHigh(internalBuffer, bufferSize, getFrequency(), getResonance());
			break;
		case Low:
			filter->processLow(internalBuffer, bufferSize, getFrequency(), getResonance());
			break;
		case Band:
			filter->processBand(internalBuffer, bufferSize, getFrequency(), getResonance());
			break;
		case Notch:
			filter->processNotch(internalBuffer, bufferSize, getFrequency(), getResonance());
			break;
		}
		for (size_t i = 0; i < bufferSize; i++) {
			int sampleIndex = bufferIteration * bufferSize + i;
			alignas(16) double sampleValues[2];
			internalBuffer[i].store_aligned(sampleValues);
			buffer.setSample(0, sampleIndex, sampleValues[0]);
			buffer.setSample(1, sampleIndex, sampleValues[1]);
		}
	}

	const size_t remainingSamples = buffer.getNumSamples() % bufferSize;
	for (size_t i = 0; i < remainingSamples; i++) {
		int sampleIndex = bufferIterationCount * bufferSize + i;
		internalBuffer[i] = Sample(buffer.getSample(0, sampleIndex), buffer.getSample(1, sampleIndex));
	}
	switch (filterType) {
	case High:
		filter->processHigh(internalBuffer, remainingSamples, getFrequency(), getResonance());
		break;
	case Low:
		filter->processLow(internalBuffer, remainingSamples, getFrequency(), getResonance());
		break;
	case Band:
		filter->processBand(internalBuffer, remainingSamples, getFrequency(), getResonance());
		break;
	case Notch:
		filter->processNotch(internalBuffer, remainingSamples, getFrequency(), getResonance());
		break;
	}
	for (size_t i = 0; i < remainingSamples; i++) {
		int sampleIndex = bufferIterationCount * bufferSize + i;
		alignas(16) double sampleValues[2];
		internalBuffer[i].store_aligned(sampleValues);
		buffer.setSample(0, sampleIndex, sampleValues[0]);
		buffer.setSample(1, sampleIndex, sampleValues[1]);
	}
}

AudioProcessorEditor* FilterAudioProcessor::createEditor()
{
	return new FilterAudioProcessorEditor(*this);
}

void FilterAudioProcessor::setResonance(FloatParameter newResonance) {
	resonance->setValueNotifyingHost(newResonance.getNormalizedValue());
}

void FilterAudioProcessor::setFrequency(FloatParameter newFrequency) {
	frequency->setValueNotifyingHost(newFrequency.getNormalizedValue());
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
	return var(&properties);
}

void FilterAudioProcessor::setState(const var & state)
{
	frequency->setValue(state.getProperty("frequency", frequency->getDefaultValue()));
	resonance->setValue(state.getProperty("resonance", resonance->getDefaultValue()));
	setFilterType(state.getProperty("filterType", 0));
}

float FilterAudioProcessor::getResonance() {
	return resonance->getValue();
}

float FilterAudioProcessor::getFrequency() {
	return frequency->getValue();
}

int FilterAudioProcessor::getFilterType() {
	return filterType;
}
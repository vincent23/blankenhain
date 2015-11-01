#include "FilterAudioProcessor.h"
#include "FilterAudioProcessorEditor.h"

FilterAudioProcessor::FilterAudioProcessor() :
	resonance("Resonance", 0.5),
	frequency("Frequency", (500.f - 40.f) / (22000.f - 40.f)),
	filterType(FilterType::Low)
{
	addParameter(&resonance);
	addParameter(&frequency);
}

void FilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void FilterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void FilterAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
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

AudioProcessorEditor* FilterAudioProcessor::createEditor()
{
	return new FilterAudioProcessorEditor(*this);
}

void FilterAudioProcessor::setResonance(float newResonance) {
	resonance.setValueNotifyingHost(newResonance);
}

void FilterAudioProcessor::setFrequency(float newFrequency) {
	float frequencyNormalized = (newFrequency - 40.f) / (22000.f - 40.f);
	frequency.setValueNotifyingHost(frequencyNormalized);
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
	properties->setProperty("frequency", frequency.getValue());
	properties->setProperty("resonance", resonance.getValue());
	properties->setProperty("filterType", filterType);
	return var(&properties);
}

void FilterAudioProcessor::setState(const var & state)
{
	frequency.setValue(state.getProperty("frequency", frequency.getDefaultValue()));
	resonance.setValue(state.getProperty("resonance", resonance.getDefaultValue()));
	setFilterType(state.getProperty("filterType", 0));
}

float FilterAudioProcessor::getResonance() {
	return resonance.getValue();
}

float FilterAudioProcessor::getFrequency() {
	return frequency.getValue() * (22000.f - 40.f) + 40.f;
}

int FilterAudioProcessor::getFilterType() {
	return filterType;
}
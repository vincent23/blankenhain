#ifndef FILTER_AUDIO_PROCESSOR_H_INCLUDED
#define FILTER_AUDIO_PROCESSOR_H_INCLUDED

#include <juce>
#include "BlankenhainAudioProcessor.h"

class FilterAudioProcessor : public BlankenhainAudioProcessor
{
public:
	FilterAudioProcessor();

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;

protected:
	var getState() override;
	void setState(const var& state) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterAudioProcessor)
};


#endif // FILTER_AUDIO_PROCESSOR_H_INCLUDED

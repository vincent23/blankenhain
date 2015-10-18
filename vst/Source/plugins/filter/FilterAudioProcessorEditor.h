#ifndef FILTER_AUDIO_PROCESSOR_EDITOR_H_INCLUDED
#define FILTER_AUDIO_PROCESSOR_EDITOR_H_INCLUDED

#include <juce>
#include "FilterAudioProcessor.h"

class FilterAudioProcessorEditor : public AudioProcessorEditor
{
public:
    FilterAudioProcessorEditor(FilterAudioProcessor&);
    ~FilterAudioProcessorEditor();

    void paint(Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FilterAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterAudioProcessorEditor)
};

#endif  // PLUGINEDITOR_H_INCLUDED

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include <juce>
#include "ExampleAudioProcessor.h"

class ExampleAudioProcessorEditor : public AudioProcessorEditor
{
public:
    ExampleAudioProcessorEditor(ExampleAudioProcessor&);
    ~ExampleAudioProcessorEditor();

    void paint(Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ExampleAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ExampleAudioProcessorEditor)
};

#endif  // PLUGINEDITOR_H_INCLUDED

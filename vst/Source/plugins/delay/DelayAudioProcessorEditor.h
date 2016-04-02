/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 4.1.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_4CF8B75E3AFD8E70__
#define __JUCE_HEADER_4CF8B75E3AFD8E70__

//[Headers]     -- You can add your own extra header files here --
#ifdef PLUGIN_DELAY
#include "JuceHeader.h"
#include "DelayAudioProcessor.h"
//[/Headers]

#include "../../components/MeterComponent.h"


//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class DelayAudioProcessorEditor  : public AudioProcessorEditor,
                                   public Timer,
                                   public SliderListener
{
public:
    //==============================================================================
    DelayAudioProcessorEditor (DelayAudioProcessor& p);
    ~DelayAudioProcessorEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void timerCallback() override;
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    DelayAudioProcessor& processor;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Slider> freeLengthSlider;
    ScopedPointer<Slider> feedbackSlider;
    ScopedPointer<Slider> panSlider;
    ScopedPointer<Label> panLabel;
    ScopedPointer<Label> feedbackLabel;
    ScopedPointer<Label> freeLengthLabel;
    ScopedPointer<MeterComponent> meterChild;
    ScopedPointer<Label> hostBPMLabel;
    ScopedPointer<Label> valueLabel;
    ScopedPointer<Slider> drywetSlider;
    ScopedPointer<Label> label;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayAudioProcessorEditor)
};

//[EndFile] You can add extra defines here...
#endif
//[/EndFile]

#endif   // __JUCE_HEADER_4CF8B75E3AFD8E70__

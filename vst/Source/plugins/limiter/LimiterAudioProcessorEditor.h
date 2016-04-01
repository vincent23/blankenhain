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

#ifndef __JUCE_HEADER_F9251F32D88354E2__
#define __JUCE_HEADER_F9251F32D88354E2__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "LimiterAudioProcessor.h"
//[/Headers]

#include "../../components/MeterComponent.h"


//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class LimiterAudioProcessorEditor  : public AudioProcessorEditor,
                                     public Timer,
                                     public SliderListener,
                                     public ButtonListener
{
public:
    //==============================================================================
    LimiterAudioProcessorEditor (LimiterAudioProcessor& p);
    ~LimiterAudioProcessorEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void timerCallback() override;
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void buttonClicked (Button* buttonThatWasClicked);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	LimiterAudioProcessor& processor;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<MeterComponent> meterChild;
    ScopedPointer<Slider> releaseSlider;
    ScopedPointer<Label> releaseText;
    ScopedPointer<Slider> lookaheadSlider;
    ScopedPointer<Label> lookaheadText;
    ScopedPointer<Slider> igainSlider;
    ScopedPointer<Slider> ogainSlider;
    ScopedPointer<Label> igainText;
    ScopedPointer<Label> ogainText;
    ScopedPointer<ToggleButton> bypassButton;
    ScopedPointer<TextButton> suggestedOgainLabel;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LimiterAudioProcessorEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_F9251F32D88354E2__

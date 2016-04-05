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

#ifndef __JUCE_HEADER_79EE6A5158B63118__
#define __JUCE_HEADER_79EE6A5158B63118__

//[Headers]     -- You can add your own extra header files here --
#ifdef PLUGIN_NONLIN_DISTORTION
#include "JuceHeader.h"
#include "NonlinDistortionAudioProcessor.h"
//[/Headers]

#include "../../components/MeterComponent.h"


//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class NonlinDistortionAudioProcessorEditor  : public AudioProcessorEditor,
                                        public Timer,
                                        public ComboBoxListener,
                                        public SliderListener
{
public:
    //==============================================================================
    NonlinDistortionAudioProcessorEditor (NonlinDistortionAudioProcessor& p);
    ~NonlinDistortionAudioProcessorEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void timerCallback() override;
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
    void sliderValueChanged (Slider* sliderThatWasMoved);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    NonlinDistortionAudioProcessor& processor;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<MeterComponent> meterChild;
    ScopedPointer<ComboBox> distortionBox;
    ScopedPointer<Slider> intensitySlider;
    ScopedPointer<Label> dsitortionLabel;
    ScopedPointer<Label> intensityLabel;
    ScopedPointer<Label> inputGainLabel;
    ScopedPointer<Slider> inputGainSlider;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NonlinDistortionAudioProcessorEditor)
};

//[EndFile] You can add extra defines here...
#endif
//[/EndFile]

#endif   // __JUCE_HEADER_79EE6A5158B63118__

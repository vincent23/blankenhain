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

#ifndef __JUCE_HEADER_CA62AE23939FFF88__
#define __JUCE_HEADER_CA62AE23939FFF88__

//[Headers]     -- You can add your own extra header files here --
#include "PanAudioProcessor.h"
#include "JuceHeader.h"
//[/Headers]

#include "../../components/MeterComponent.h"


//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class PanAudioProcessorEditor  : public AudioProcessorEditor,
                                 public Timer,
                                 public SliderListener
{
public:
    //==============================================================================
    PanAudioProcessorEditor (PanAudioProcessor& p);
    ~PanAudioProcessorEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void timerCallback() override;
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	PanAudioProcessor& processor;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<MeterComponent> meterChild;
    ScopedPointer<Slider> panningSlider;
    ScopedPointer<Label> label;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PanAudioProcessorEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_CA62AE23939FFF88__

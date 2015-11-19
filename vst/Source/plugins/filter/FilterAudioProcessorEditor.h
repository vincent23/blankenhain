/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 4.0.1

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_80A7691534793726__
#define __JUCE_HEADER_80A7691534793726__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "FilterAudioProcessor.h"
//[/Headers]

#include "../../components/MeterComponent.h"


//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class FilterAudioProcessorEditor  : public AudioProcessorEditor,
                                    public Timer,
                                    public ComboBoxListener,
                                    public SliderListener
{
public:
    //==============================================================================
    FilterAudioProcessorEditor (FilterAudioProcessor& p);
    ~FilterAudioProcessorEditor();

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
    FilterAudioProcessor& processor;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Label> label;
    ScopedPointer<ComboBox> filterTypeSelector;
    ScopedPointer<Slider> frequencySlider;
    ScopedPointer<Slider> resonanceSlider;
    ScopedPointer<Label> label2;
    ScopedPointer<Label> label3;
    ScopedPointer<MeterComponent> meterChild;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterAudioProcessorEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_80A7691534793726__

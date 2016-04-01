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

#ifndef __JUCE_HEADER_A1B1A6D1AFC81DAA__
#define __JUCE_HEADER_A1B1A6D1AFC81DAA__

//[Headers]     -- You can add your own extra header files here --
#ifdef PLUGIN_EQUALIZER
#include "JuceHeader.h"
#include "EqualizerAudioProcessor.h"
//[/Headers]

#include "../../components/MeterComponent.h"


//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class EqualizerAudioProcessorEditor  : public AudioProcessorEditor,
                                       public Timer,
                                       public SliderListener
{
public:
    //==============================================================================
    EqualizerAudioProcessorEditor (EqualizerAudioProcessor& p);
    ~EqualizerAudioProcessorEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void timerCallback() override;
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    EqualizerAudioProcessor& processor;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Slider> lowSlider;
    ScopedPointer<Slider> midSlider;
    ScopedPointer<Slider> highFreqSlider;
    ScopedPointer<Slider> lowFreqSlider;
    ScopedPointer<Slider> highSlider;
    ScopedPointer<Label> highLabel;
    ScopedPointer<Label> highFreqLabel;
    ScopedPointer<Label> midLabel;
    ScopedPointer<Label> lowFreqLabel;
    ScopedPointer<Label> lowLabel;
    ScopedPointer<MeterComponent> meterChild;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EqualizerAudioProcessorEditor)
};

//[EndFile] You can add extra defines here...
#endif
//[/EndFile]

#endif   // __JUCE_HEADER_A1B1A6D1AFC81DAA__

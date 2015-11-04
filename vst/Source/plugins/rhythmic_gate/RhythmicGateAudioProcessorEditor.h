/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.2.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_453765CA9928EEBC__
#define __JUCE_HEADER_453765CA9928EEBC__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class RhythmicGateAudioProcessorEditor  : public Component,
                                          public SliderListener
{
public:
    //==============================================================================
    RhythmicGateAudioProcessorEditor ();
    ~RhythmicGateAudioProcessorEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Slider> slider;
    ScopedPointer<Slider> slider2;
    ScopedPointer<Slider> slider3;
    ScopedPointer<Slider> slider4;
    ScopedPointer<Slider> slider5;
    ScopedPointer<Slider> slider6;
    ScopedPointer<Slider> slider7;
    ScopedPointer<Slider> slider8;
    ScopedPointer<Slider> slider9;
    ScopedPointer<Slider> slider10;
    ScopedPointer<Slider> slider11;
    ScopedPointer<Slider> slider12;
    ScopedPointer<Slider> slider13;
    ScopedPointer<Slider> slider14;
    ScopedPointer<Slider> slider15;
    ScopedPointer<Slider> slider16;
    ScopedPointer<Slider> slider17;
    ScopedPointer<Slider> slider18;
    ScopedPointer<Slider> slider19;
    ScopedPointer<Label> label;
    ScopedPointer<Label> label2;
    ScopedPointer<Label> label3;
    ScopedPointer<Label> label4;
    ScopedPointer<Label> label5;
    ScopedPointer<Slider> slider20;
    ScopedPointer<Label> label6;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RhythmicGateAudioProcessorEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_453765CA9928EEBC__

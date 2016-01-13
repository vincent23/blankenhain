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

#ifndef __JUCE_HEADER_B1EBD55EE2E96BAE__
#define __JUCE_HEADER_B1EBD55EE2E96BAE__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "KickAudioProcessor.h"
//[/Headers]

#include "../../components/MeterComponent.h"


//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class KickAudioProcessorEditor  : public AudioProcessorEditor,
                                  public Timer
{
public:
    //==============================================================================
    KickAudioProcessorEditor (KickAudioProcessor& p);
    ~KickAudioProcessorEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void timerCallback() override;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	KickAudioProcessor& processor;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<MeterComponent> meterComponent;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KickAudioProcessorEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_B1EBD55EE2E96BAE__

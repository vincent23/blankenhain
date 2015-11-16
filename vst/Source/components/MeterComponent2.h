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

#ifndef __JUCE_HEADER_C1C2B6D0C586EE72__
#define __JUCE_HEADER_C1C2B6D0C586EE72__

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
class MeterComponent  : public Component
{
public:
    //==============================================================================
    MeterComponent ();
    ~MeterComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void setValue(float*);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Label> curLeft;
    ScopedPointer<Label> curRight;
    ScopedPointer<Label> peakLeft;
    ScopedPointer<Label> peakRight;
    ScopedPointer<Label> RMSLeft;
    ScopedPointer<Label> RMSRight;
    ScopedPointer<Label> curText;
    ScopedPointer<Label> peakText;
    ScopedPointer<Label> rmsText;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MeterComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_C1C2B6D0C586EE72__

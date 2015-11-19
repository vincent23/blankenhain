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

#ifndef __JUCE_HEADER_768331F7EDADB140__
#define __JUCE_HEADER_768331F7EDADB140__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "ledMeterComponent.h"
//[/Headers]

#include "ledMeterComponent.h"


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

	//USE THIS TO PROCESS THE float-vec "in" from the processor.
	//do not call peakFollower
	void setValue(std::vector<float> in, double sampleRate = 44100, int blockSize = 512);


	void mouseDown(const MouseEvent& mouseIn) override;

	// this is called from inside setValue
	// dont use this manually
	void peakFollower(std::vector<float> newValues, double sampleRate = 44100, int blockSize = 512);

    //[/UserMethods]

    void paint (Graphics& g);
    void resized();



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	std::vector<float> bareValues;
	std::vector<float> decayingValues;
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
    ScopedPointer<ledPeakMeterComponent> ledPeakMeterChild;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MeterComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_768331F7EDADB140__

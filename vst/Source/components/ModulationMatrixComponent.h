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

#ifndef __JUCE_HEADER_7932748FC001F0E8__
#define __JUCE_HEADER_7932748FC001F0E8__

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
class ModulationMatrixComponent  : public Component,
                                   public SliderListener,
                                   public ComboBoxListener
{
public:
    //==============================================================================
    ModulationMatrixComponent ();
    ~ModulationMatrixComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Slider> slider;
    ScopedPointer<ComboBox> comboBox3;
    ScopedPointer<ComboBox> comboBox2;
    ScopedPointer<Slider> slider2;
    ScopedPointer<ComboBox> comboBox4;
    ScopedPointer<ComboBox> comboBox5;
    ScopedPointer<Slider> slider3;
    ScopedPointer<ComboBox> comboBox6;
    ScopedPointer<ComboBox> comboBox7;
    ScopedPointer<Slider> slider4;
    ScopedPointer<ComboBox> comboBox8;
    ScopedPointer<ComboBox> comboBox9;
    ScopedPointer<Slider> slider5;
    ScopedPointer<ComboBox> comboBox10;
    ScopedPointer<ComboBox> comboBox11;
    ScopedPointer<Slider> slider6;
    ScopedPointer<ComboBox> comboBox12;
    ScopedPointer<ComboBox> comboBox13;
    ScopedPointer<Slider> slider7;
    ScopedPointer<ComboBox> comboBox14;
    ScopedPointer<ComboBox> comboBox15;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModulationMatrixComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_7932748FC001F0E8__

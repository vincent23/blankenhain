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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "MeterComponent2.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MeterComponent::MeterComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (curLeft = new Label ("curLeft",
                                            TRANS("? dB")));
    curLeft->setFont (Font (15.00f, Font::plain));
    curLeft->setJustificationType (Justification::centred);
    curLeft->setEditable (false, false, false);
    curLeft->setColour (Label::backgroundColourId, Colour (0xff7e7e7e));
    curLeft->setColour (TextEditor::textColourId, Colours::black);
    curLeft->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (curRight = new Label ("curRight",
                                             TRANS("? dB")));
    curRight->setFont (Font (15.00f, Font::plain));
    curRight->setJustificationType (Justification::centred);
    curRight->setEditable (false, false, false);
    curRight->setColour (Label::backgroundColourId, Colour (0xff7e7e7e));
    curRight->setColour (TextEditor::textColourId, Colours::black);
    curRight->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (peakLeft = new Label ("peakLeft",
                                             TRANS("? dB")));
    peakLeft->setFont (Font (15.00f, Font::plain));
    peakLeft->setJustificationType (Justification::centred);
    peakLeft->setEditable (false, false, false);
    peakLeft->setColour (Label::backgroundColourId, Colour (0xff7e7e7e));
    peakLeft->setColour (TextEditor::textColourId, Colours::black);
    peakLeft->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (peakRight = new Label ("peakRight",
                                              TRANS("? dB")));
    peakRight->setFont (Font (15.00f, Font::plain));
    peakRight->setJustificationType (Justification::centred);
    peakRight->setEditable (false, false, false);
    peakRight->setColour (Label::backgroundColourId, Colour (0xff7e7e7e));
    peakRight->setColour (TextEditor::textColourId, Colours::black);
    peakRight->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (RMSLeft = new Label ("RMSLeft",
                                            TRANS("? dB")));
    RMSLeft->setFont (Font (15.00f, Font::plain));
    RMSLeft->setJustificationType (Justification::centred);
    RMSLeft->setEditable (false, false, false);
    RMSLeft->setColour (Label::backgroundColourId, Colour (0xff7e7e7e));
    RMSLeft->setColour (TextEditor::textColourId, Colours::black);
    RMSLeft->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (RMSRight = new Label ("RMSRight",
                                             TRANS("? dB")));
    RMSRight->setFont (Font (15.00f, Font::plain));
    RMSRight->setJustificationType (Justification::centred);
    RMSRight->setEditable (false, false, false);
    RMSRight->setColour (Label::backgroundColourId, Colour (0xff7e7e7e));
    RMSRight->setColour (TextEditor::textColourId, Colours::black);
    RMSRight->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (curText = new Label ("curText",
                                            TRANS("cur")));
    curText->setFont (Font (15.00f, Font::plain));
    curText->setJustificationType (Justification::centred);
    curText->setEditable (false, false, false);
    curText->setColour (Label::backgroundColourId, Colours::black);
    curText->setColour (Label::textColourId, Colours::white);
    curText->setColour (Label::outlineColourId, Colours::white);
    curText->setColour (TextEditor::textColourId, Colours::black);
    curText->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (peakText = new Label ("peakText",
                                             TRANS("Peak")));
    peakText->setFont (Font (15.00f, Font::plain));
    peakText->setJustificationType (Justification::centred);
    peakText->setEditable (false, false, false);
    peakText->setColour (Label::backgroundColourId, Colours::black);
    peakText->setColour (Label::textColourId, Colours::white);
    peakText->setColour (Label::outlineColourId, Colours::white);
    peakText->setColour (TextEditor::textColourId, Colours::black);
    peakText->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (rmsText = new Label ("rmsText",
                                            TRANS("RMS")));
    rmsText->setFont (Font (15.00f, Font::plain));
    rmsText->setJustificationType (Justification::centred);
    rmsText->setEditable (false, false, false);
    rmsText->setColour (Label::backgroundColourId, Colours::black);
    rmsText->setColour (Label::textColourId, Colours::white);
    rmsText->setColour (Label::outlineColourId, Colours::white);
    rmsText->setColour (TextEditor::textColourId, Colours::black);
    rmsText->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (300, 200);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

MeterComponent::~MeterComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    curLeft = nullptr;
    curRight = nullptr;
    peakLeft = nullptr;
    peakRight = nullptr;
    RMSLeft = nullptr;
    RMSRight = nullptr;
    curText = nullptr;
    peakText = nullptr;
    rmsText = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MeterComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::black);

    g.setColour (Colours::black);
    g.fillRect (120, 10, 40, 182);

    g.setColour (Colours::white);
    g.drawRect (120, 10, 40, 182, 5);

    g.setColour (Colours::black);
    g.fillRect (8, 10, 40, 182);

    g.setColour (Colours::white);
    g.drawRect (8, 10, 40, 182, 5);

    g.setGradientFill (ColourGradient (Colours::red,
                                       0.0f, 0.0f,
                                       Colours::green,
                                       0.0f, 100.0f,
                                       false));
    g.fillRect (13, 15, 30, 174);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (60, 180, 20, 8);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (60, 164, 20, 8);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (60, 148, 20, 8);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (60, 132, 20, 8);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (60, 116, 20, 8);

    g.setColour (Colour (0xffff2a9f));
    g.fillRect (60, 100, 20, 8);

    g.setColour (Colour (0xffff2a9f));
    g.fillRect (60, 84, 20, 8);

    g.setColour (Colour (0xffff2a9f));
    g.fillRect (60, 68, 20, 8);

    g.setColour (Colour (0xffff9c9f));
    g.fillRect (60, 52, 20, 8);

    g.setColour (Colour (0xffff9c9f));
    g.fillRect (60, 36, 20, 8);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (92, 180, 20, 8);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (92, 164, 20, 8);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (92, 148, 20, 8);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (92, 132, 20, 8);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (92, 116, 20, 8);

    g.setColour (Colour (0xffff2a9f));
    g.fillRect (92, 100, 20, 8);

    g.setColour (Colour (0xffff2a9f));
    g.fillRect (92, 84, 20, 8);

    g.setColour (Colour (0xffff2a9f));
    g.fillRect (92, 68, 20, 8);

    g.setColour (Colour (0xffff9c9f));
    g.fillRect (92, 52, 20, 8);

    g.setColour (Colour (0xffff9c9f));
    g.fillRect (92, 36, 20, 8);

    g.setColour (Colour (0xffffcd9f));
    g.fillRect (92, 20, 20, 8);

    g.setColour (Colour (0xffffcd9f));
    g.fillRect (60, 20, 20, 8);

    g.setGradientFill (ColourGradient (Colours::red,
                                       0.0f, 0.0f,
                                       Colours::green,
                                       0.0f, 100.0f,
                                       false));
    g.fillRect (125, 15, 30, 174);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MeterComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    curLeft->setBounds (168, 16, 40, 32);
    curRight->setBounds (248, 16, 40, 32);
    peakLeft->setBounds (168, 64, 40, 32);
    peakRight->setBounds (248, 64, 40, 32);
    RMSLeft->setBounds (168, 112, 40, 32);
    RMSRight->setBounds (248, 112, 40, 32);
    curText->setBounds (208, 16, 40, 32);
    peakText->setBounds (208, 64, 40, 32);
    rmsText->setBounds (208, 112, 40, 32);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void MeterComponent::setValue(float* meterValues)
{
	this->curLeft->setText((juce::String) meterValues[0] + " dB", juce::dontSendNotification);
	this->curRight->setText((juce::String) meterValues[1] + " dB", juce::dontSendNotification);
	this->peakLeft->setText((juce::String) meterValues[2] + " dB", juce::dontSendNotification);
	this->peakRight->setText((juce::String) meterValues[3] + " dB", juce::dontSendNotification);
	this->RMSLeft->setText((juce::String) meterValues[4] + " dB", juce::dontSendNotification);
	this->RMSRight->setText((juce::String) meterValues[5] + " dB", juce::dontSendNotification);
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MeterComponent" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="300" initialHeight="200">
  <BACKGROUND backgroundColour="ff000000">
    <RECT pos="120 10 40 182" fill="solid: ff000000" hasStroke="1" stroke="5, mitered, butt"
          strokeColour="solid: ffffffff"/>
    <RECT pos="8 10 40 182" fill="solid: ff000000" hasStroke="1" stroke="5, mitered, butt"
          strokeColour="solid: ffffffff"/>
    <RECT pos="13 15 30 174" fill="linear: 0 0, 0 100, 0=ffff0000, 1=ff008000"
          hasStroke="0"/>
    <RECT pos="60 180 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="60 164 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="60 148 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="60 132 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="60 116 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="60 100 20 8" fill="solid: ffff2a9f" hasStroke="0"/>
    <RECT pos="60 84 20 8" fill="solid: ffff2a9f" hasStroke="0"/>
    <RECT pos="60 68 20 8" fill="solid: ffff2a9f" hasStroke="0"/>
    <RECT pos="60 52 20 8" fill="solid: ffff9c9f" hasStroke="0"/>
    <RECT pos="60 36 20 8" fill="solid: ffff9c9f" hasStroke="0"/>
    <RECT pos="92 180 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="92 164 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="92 148 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="92 132 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="92 116 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="92 100 20 8" fill="solid: ffff2a9f" hasStroke="0"/>
    <RECT pos="92 84 20 8" fill="solid: ffff2a9f" hasStroke="0"/>
    <RECT pos="92 68 20 8" fill="solid: ffff2a9f" hasStroke="0"/>
    <RECT pos="92 52 20 8" fill="solid: ffff9c9f" hasStroke="0"/>
    <RECT pos="92 36 20 8" fill="solid: ffff9c9f" hasStroke="0"/>
    <RECT pos="92 20 20 8" fill="solid: ffffcd9f" hasStroke="0"/>
    <RECT pos="60 20 20 8" fill="solid: ffffcd9f" hasStroke="0"/>
    <RECT pos="125 15 30 174" fill="linear: 0 0, 0 100, 0=ffff0000, 1=ff008000"
          hasStroke="0"/>
  </BACKGROUND>
  <LABEL name="curLeft" id="e44918e7c6b44efc" memberName="curLeft" virtualName=""
         explicitFocusOrder="0" pos="168 16 40 32" bkgCol="ff7e7e7e" edTextCol="ff000000"
         edBkgCol="0" labelText="? dB" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <LABEL name="curRight" id="40907e8926b60d92" memberName="curRight" virtualName=""
         explicitFocusOrder="0" pos="248 16 40 32" bkgCol="ff7e7e7e" edTextCol="ff000000"
         edBkgCol="0" labelText="? dB" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <LABEL name="peakLeft" id="fb5189acf29f5db8" memberName="peakLeft" virtualName=""
         explicitFocusOrder="0" pos="168 64 40 32" bkgCol="ff7e7e7e" edTextCol="ff000000"
         edBkgCol="0" labelText="? dB" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <LABEL name="peakRight" id="d7f00102b0c6cb6b" memberName="peakRight"
         virtualName="" explicitFocusOrder="0" pos="248 64 40 32" bkgCol="ff7e7e7e"
         edTextCol="ff000000" edBkgCol="0" labelText="? dB" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="RMSLeft" id="31d2229f458b750" memberName="RMSLeft" virtualName=""
         explicitFocusOrder="0" pos="168 112 40 32" bkgCol="ff7e7e7e"
         edTextCol="ff000000" edBkgCol="0" labelText="? dB" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="RMSRight" id="b6e6d28afd03ec9" memberName="RMSRight" virtualName=""
         explicitFocusOrder="0" pos="248 112 40 32" bkgCol="ff7e7e7e"
         edTextCol="ff000000" edBkgCol="0" labelText="? dB" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="curText" id="424a38d38b34428b" memberName="curText" virtualName=""
         explicitFocusOrder="0" pos="208 16 40 32" bkgCol="ff000000" textCol="ffffffff"
         outlineCol="ffffffff" edTextCol="ff000000" edBkgCol="0" labelText="cur"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="peakText" id="f3dea9733105c26d" memberName="peakText" virtualName=""
         explicitFocusOrder="0" pos="208 64 40 32" bkgCol="ff000000" textCol="ffffffff"
         outlineCol="ffffffff" edTextCol="ff000000" edBkgCol="0" labelText="Peak"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="rmsText" id="643eeda53297a283" memberName="rmsText" virtualName=""
         explicitFocusOrder="0" pos="208 112 40 32" bkgCol="ff000000"
         textCol="ffffffff" outlineCol="ffffffff" edTextCol="ff000000"
         edBkgCol="0" labelText="RMS" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

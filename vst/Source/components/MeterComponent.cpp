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

#include "MeterComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MeterComponent::MeterComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (curLeft = new Label ("new label",
                                            TRANS("label text")));
    curLeft->setFont (Font (15.00f, Font::plain));
    curLeft->setJustificationType (Justification::centredLeft);
    curLeft->setEditable (false, false, false);
    curLeft->setColour (Label::backgroundColourId, Colour (0xff7e7e7e));
    curLeft->setColour (TextEditor::textColourId, Colours::black);
    curLeft->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (curRight = new Label ("new label",
                                             TRANS("label text")));
    curRight->setFont (Font (15.00f, Font::plain));
    curRight->setJustificationType (Justification::centredLeft);
    curRight->setEditable (false, false, false);
    curRight->setColour (Label::backgroundColourId, Colour (0xff7e7e7e));
    curRight->setColour (TextEditor::textColourId, Colours::black);
    curRight->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (peakLeft = new Label ("new label",
                                             TRANS("label text")));
    peakLeft->setFont (Font (15.00f, Font::plain));
    peakLeft->setJustificationType (Justification::centredLeft);
    peakLeft->setEditable (false, false, false);
    peakLeft->setColour (Label::backgroundColourId, Colour (0xff7e7e7e));
    peakLeft->setColour (TextEditor::textColourId, Colours::black);
    peakLeft->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (peakRight = new Label ("new label",
                                              TRANS("label text")));
    peakRight->setFont (Font (15.00f, Font::plain));
    peakRight->setJustificationType (Justification::centredLeft);
    peakRight->setEditable (false, false, false);
    peakRight->setColour (Label::backgroundColourId, Colour (0xff7e7e7e));
    peakRight->setColour (TextEditor::textColourId, Colours::black);
    peakRight->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (RMSLeft = new Label ("new label",
                                            TRANS("label text")));
    RMSLeft->setFont (Font (15.00f, Font::plain));
    RMSLeft->setJustificationType (Justification::centredLeft);
    RMSLeft->setEditable (false, false, false);
    RMSLeft->setColour (Label::backgroundColourId, Colour (0xff7e7e7e));
    RMSLeft->setColour (TextEditor::textColourId, Colours::black);
    RMSLeft->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (RMSRight = new Label ("new label",
                                             TRANS("label text")));
    RMSRight->setFont (Font (15.00f, Font::plain));
    RMSRight->setJustificationType (Justification::centredLeft);
    RMSRight->setEditable (false, false, false);
    RMSRight->setColour (Label::backgroundColourId, Colour (0xff7e7e7e));
    RMSRight->setColour (TextEditor::textColourId, Colours::black);
    RMSRight->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (curText = new Label ("new label",
                                            TRANS("cur")));
    curText->setFont (Font (15.00f, Font::plain));
    curText->setJustificationType (Justification::centred);
    curText->setEditable (false, false, false);
    curText->setColour (Label::backgroundColourId, Colours::black);
    curText->setColour (Label::textColourId, Colours::white);
    curText->setColour (Label::outlineColourId, Colours::white);
    curText->setColour (TextEditor::textColourId, Colours::black);
    curText->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (curText2 = new Label ("new label",
                                             TRANS("Peak")));
    curText2->setFont (Font (15.00f, Font::plain));
    curText2->setJustificationType (Justification::centred);
    curText2->setEditable (false, false, false);
    curText2->setColour (Label::backgroundColourId, Colours::black);
    curText2->setColour (Label::textColourId, Colours::white);
    curText2->setColour (Label::outlineColourId, Colours::white);
    curText2->setColour (TextEditor::textColourId, Colours::black);
    curText2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (curText3 = new Label ("new label",
                                             TRANS("RMS")));
    curText3->setFont (Font (15.00f, Font::plain));
    curText3->setJustificationType (Justification::centred);
    curText3->setEditable (false, false, false);
    curText3->setColour (Label::backgroundColourId, Colours::black);
    curText3->setColour (Label::textColourId, Colours::white);
    curText3->setColour (Label::outlineColourId, Colours::white);
    curText3->setColour (TextEditor::textColourId, Colours::black);
    curText3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (250, 420);


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
    curText2 = nullptr;
    curText3 = nullptr;


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
    g.fillRect (162, 10, 40, 280);

    g.setColour (Colours::white);
    g.drawRect (162, 10, 40, 280, 5);

    g.setColour (Colours::black);
    g.fillRect (50, 10, 40, 280);

    g.setColour (Colours::white);
    g.drawRect (50, 10, 40, 280, 5);

    g.setGradientFill (ColourGradient (Colours::red,
                                       0.0f, 0.0f,
                                       Colours::green,
                                       0.0f, 100.0f,
                                       false));
    g.fillRect (52, 12, 36, 276);

    g.setGradientFill (ColourGradient (Colours::red,
                                       0.0f, 0.0f,
                                       Colours::green,
                                       0.0f, 100.0f,
                                       false));
    g.fillRect (164, 12, 36, 276);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (100, 276, 20, 8);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (100, 260, 20, 8);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (100, 244, 20, 8);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (100, 228, 20, 8);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (100, 212, 20, 8);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (100, 196, 20, 8);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (100, 180, 20, 8);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (100, 164, 20, 8);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (100, 148, 20, 8);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (100, 132, 20, 8);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (100, 116, 20, 8);

    g.setColour (Colour (0xffff2a9f));
    g.fillRect (100, 100, 20, 8);

    g.setColour (Colour (0xffff2a9f));
    g.fillRect (100, 84, 20, 8);

    g.setColour (Colour (0xffff2a9f));
    g.fillRect (100, 68, 20, 8);

    g.setColour (Colour (0xffff9c9f));
    g.fillRect (100, 52, 20, 8);

    g.setColour (Colour (0xffff9c9f));
    g.fillRect (100, 36, 20, 8);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (132, 276, 20, 8);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (132, 260, 20, 8);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (132, 244, 20, 8);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (132, 228, 20, 8);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (132, 212, 20, 8);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (132, 196, 20, 8);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (132, 180, 20, 8);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (132, 164, 20, 8);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (132, 148, 20, 8);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (132, 132, 20, 8);

    g.setColour (Colour (0xffa52a9f));
    g.fillRect (132, 116, 20, 8);

    g.setColour (Colour (0xffff2a9f));
    g.fillRect (132, 100, 20, 8);

    g.setColour (Colour (0xffff2a9f));
    g.fillRect (132, 84, 20, 8);

    g.setColour (Colour (0xffff2a9f));
    g.fillRect (132, 68, 20, 8);

    g.setColour (Colour (0xffff9c9f));
    g.fillRect (132, 52, 20, 8);

    g.setColour (Colour (0xffff9c9f));
    g.fillRect (132, 36, 20, 8);

    g.setColour (Colour (0xffffcd9f));
    g.fillRect (132, 20, 20, 8);

    g.setColour (Colour (0xffffcd9f));
    g.fillRect (100, 20, 20, 8);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MeterComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    curLeft->setBounds (16, 296, 80, 32);
    curRight->setBounds (152, 296, 79, 32);
    peakLeft->setBounds (16, 336, 80, 32);
    peakRight->setBounds (152, 336, 79, 32);
    RMSLeft->setBounds (16, 376, 80, 32);
    RMSRight->setBounds (152, 376, 79, 32);
    curText->setBounds (104, 296, 40, 32);
    curText2->setBounds (104, 336, 40, 32);
    curText3->setBounds (104, 376, 40, 32);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
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
                 fixedSize="1" initialWidth="250" initialHeight="420">
  <BACKGROUND backgroundColour="ff000000">
    <RECT pos="162 10 40 280" fill="solid: ff000000" hasStroke="1" stroke="5, mitered, butt"
          strokeColour="solid: ffffffff"/>
    <RECT pos="50 10 40 280" fill="solid: ff000000" hasStroke="1" stroke="5, mitered, butt"
          strokeColour="solid: ffffffff"/>
    <RECT pos="52 12 36 276" fill="linear: 0 0, 0 100, 0=ffff0000, 1=ff008000"
          hasStroke="0"/>
    <RECT pos="164 12 36 276" fill="linear: 0 0, 0 100, 0=ffff0000, 1=ff008000"
          hasStroke="0"/>
    <RECT pos="100 276 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="100 260 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="100 244 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="100 228 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="100 212 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="100 196 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="100 180 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="100 164 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="100 148 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="100 132 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="100 116 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="100 100 20 8" fill="solid: ffff2a9f" hasStroke="0"/>
    <RECT pos="100 84 20 8" fill="solid: ffff2a9f" hasStroke="0"/>
    <RECT pos="100 68 20 8" fill="solid: ffff2a9f" hasStroke="0"/>
    <RECT pos="100 52 20 8" fill="solid: ffff9c9f" hasStroke="0"/>
    <RECT pos="100 36 20 8" fill="solid: ffff9c9f" hasStroke="0"/>
    <RECT pos="132 276 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="132 260 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="132 244 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="132 228 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="132 212 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="132 196 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="132 180 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="132 164 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="132 148 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="132 132 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="132 116 20 8" fill="solid: ffa52a9f" hasStroke="0"/>
    <RECT pos="132 100 20 8" fill="solid: ffff2a9f" hasStroke="0"/>
    <RECT pos="132 84 20 8" fill="solid: ffff2a9f" hasStroke="0"/>
    <RECT pos="132 68 20 8" fill="solid: ffff2a9f" hasStroke="0"/>
    <RECT pos="132 52 20 8" fill="solid: ffff9c9f" hasStroke="0"/>
    <RECT pos="132 36 20 8" fill="solid: ffff9c9f" hasStroke="0"/>
    <RECT pos="132 20 20 8" fill="solid: ffffcd9f" hasStroke="0"/>
    <RECT pos="100 20 20 8" fill="solid: ffffcd9f" hasStroke="0"/>
  </BACKGROUND>
  <LABEL name="new label" id="e44918e7c6b44efc" memberName="curLeft" virtualName=""
         explicitFocusOrder="0" pos="16 296 80 32" bkgCol="ff7e7e7e" edTextCol="ff000000"
         edBkgCol="0" labelText="label text" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="40907e8926b60d92" memberName="curRight"
         virtualName="" explicitFocusOrder="0" pos="152 296 79 32" bkgCol="ff7e7e7e"
         edTextCol="ff000000" edBkgCol="0" labelText="label text" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="fb5189acf29f5db8" memberName="peakLeft"
         virtualName="" explicitFocusOrder="0" pos="16 336 80 32" bkgCol="ff7e7e7e"
         edTextCol="ff000000" edBkgCol="0" labelText="label text" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="d7f00102b0c6cb6b" memberName="peakRight"
         virtualName="" explicitFocusOrder="0" pos="152 336 79 32" bkgCol="ff7e7e7e"
         edTextCol="ff000000" edBkgCol="0" labelText="label text" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="31d2229f458b750" memberName="RMSLeft" virtualName=""
         explicitFocusOrder="0" pos="16 376 80 32" bkgCol="ff7e7e7e" edTextCol="ff000000"
         edBkgCol="0" labelText="label text" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="b6e6d28afd03ec9" memberName="RMSRight" virtualName=""
         explicitFocusOrder="0" pos="152 376 79 32" bkgCol="ff7e7e7e"
         edTextCol="ff000000" edBkgCol="0" labelText="label text" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="424a38d38b34428b" memberName="curText" virtualName=""
         explicitFocusOrder="0" pos="104 296 40 32" bkgCol="ff000000"
         textCol="ffffffff" outlineCol="ffffffff" edTextCol="ff000000"
         edBkgCol="0" labelText="cur" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="f3dea9733105c26d" memberName="curText2"
         virtualName="" explicitFocusOrder="0" pos="104 336 40 32" bkgCol="ff000000"
         textCol="ffffffff" outlineCol="ffffffff" edTextCol="ff000000"
         edBkgCol="0" labelText="Peak" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="643eeda53297a283" memberName="curText3"
         virtualName="" explicitFocusOrder="0" pos="104 376 40 32" bkgCol="ff000000"
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

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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "RhythmicGateAudioProcessorEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
RhythmicGateAudioProcessorEditor::RhythmicGateAudioProcessorEditor ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (slider = new Slider ("new slider"));
    slider->setRange (0, 1, 0.01);
    slider->setSliderStyle (Slider::LinearBar);
    slider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider->setColour (Slider::backgroundColourId, Colour (0x4ef78181));
    slider->addListener (this);

    addAndMakeVisible (slider2 = new Slider ("new slider"));
    slider2->setRange (0, 1, 0.01);
    slider2->setSliderStyle (Slider::LinearBar);
    slider2->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider2->addListener (this);

    addAndMakeVisible (slider3 = new Slider ("new slider"));
    slider3->setRange (0, 1, 0.01);
    slider3->setSliderStyle (Slider::LinearBar);
    slider3->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider3->addListener (this);

    addAndMakeVisible (slider4 = new Slider ("new slider"));
    slider4->setRange (0, 1, 0.01);
    slider4->setSliderStyle (Slider::LinearBar);
    slider4->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider4->addListener (this);

    addAndMakeVisible (slider5 = new Slider ("new slider"));
    slider5->setRange (0, 1, 0.01);
    slider5->setSliderStyle (Slider::LinearBar);
    slider5->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider5->setColour (Slider::backgroundColourId, Colour (0x4ef78181));
    slider5->addListener (this);

    addAndMakeVisible (slider6 = new Slider ("new slider"));
    slider6->setRange (0, 1, 0.01);
    slider6->setSliderStyle (Slider::LinearBar);
    slider6->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider6->addListener (this);

    addAndMakeVisible (slider7 = new Slider ("new slider"));
    slider7->setRange (0, 1, 0.01);
    slider7->setSliderStyle (Slider::LinearBar);
    slider7->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider7->addListener (this);

    addAndMakeVisible (slider8 = new Slider ("new slider"));
    slider8->setRange (0, 1, 0.01);
    slider8->setSliderStyle (Slider::LinearBar);
    slider8->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider8->addListener (this);

    addAndMakeVisible (slider9 = new Slider ("new slider"));
    slider9->setRange (0, 1, 0.01);
    slider9->setSliderStyle (Slider::LinearBar);
    slider9->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider9->setColour (Slider::backgroundColourId, Colour (0x4ef78181));
    slider9->addListener (this);

    addAndMakeVisible (slider10 = new Slider ("new slider"));
    slider10->setRange (0, 1, 0.01);
    slider10->setSliderStyle (Slider::LinearBar);
    slider10->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider10->addListener (this);

    addAndMakeVisible (slider11 = new Slider ("new slider"));
    slider11->setRange (0, 1, 0.01);
    slider11->setSliderStyle (Slider::LinearBar);
    slider11->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider11->addListener (this);

    addAndMakeVisible (slider12 = new Slider ("new slider"));
    slider12->setRange (0, 1, 0.01);
    slider12->setSliderStyle (Slider::LinearBar);
    slider12->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider12->addListener (this);

    addAndMakeVisible (slider13 = new Slider ("new slider"));
    slider13->setRange (0, 1, 0.01);
    slider13->setSliderStyle (Slider::LinearBar);
    slider13->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider13->setColour (Slider::backgroundColourId, Colour (0x4ef78181));
    slider13->addListener (this);

    addAndMakeVisible (slider14 = new Slider ("new slider"));
    slider14->setRange (0, 1, 0.01);
    slider14->setSliderStyle (Slider::LinearBar);
    slider14->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider14->addListener (this);

    addAndMakeVisible (slider15 = new Slider ("new slider"));
    slider15->setRange (0, 1, 0.01);
    slider15->setSliderStyle (Slider::LinearBar);
    slider15->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider15->addListener (this);

    addAndMakeVisible (slider16 = new Slider ("new slider"));
    slider16->setRange (0, 1, 0.01);
    slider16->setSliderStyle (Slider::LinearBar);
    slider16->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider16->addListener (this);

    addAndMakeVisible (slider17 = new Slider ("new slider"));
    slider17->setRange (0, 10, 0);
    slider17->setSliderStyle (Slider::RotaryVerticalDrag);
    slider17->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider17->addListener (this);

    addAndMakeVisible (slider18 = new Slider ("new slider"));
    slider18->setRange (0, 10, 0);
    slider18->setSliderStyle (Slider::RotaryVerticalDrag);
    slider18->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider18->addListener (this);

    addAndMakeVisible (slider19 = new Slider ("new slider"));
    slider19->setRange (0, 10, 0);
    slider19->setSliderStyle (Slider::RotaryVerticalDrag);
    slider19->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider19->addListener (this);

    addAndMakeVisible (label = new Label ("new label",
                                          TRANS("Attack")));
    label->setFont (Font (15.00f, Font::plain));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label2 = new Label ("new label",
                                           TRANS("Release")));
    label2->setFont (Font (15.00f, Font::plain));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label3 = new Label ("new label",
                                           TRANS("Dry/Wet")));
    label3->setFont (Font (15.00f, Font::plain));
    label3->setJustificationType (Justification::centredLeft);
    label3->setEditable (false, false, false);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label4 = new Label ("new label",
                                           TRANS("Step 1-8")));
    label4->setFont (Font (15.00f, Font::plain));
    label4->setJustificationType (Justification::centredLeft);
    label4->setEditable (false, false, false);
    label4->setColour (TextEditor::textColourId, Colours::black);
    label4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label5 = new Label ("new label",
                                           TRANS("Step 9-16")));
    label5->setFont (Font (15.00f, Font::plain));
    label5->setJustificationType (Justification::centredLeft);
    label5->setEditable (false, false, false);
    label5->setColour (TextEditor::textColourId, Colours::black);
    label5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider20 = new Slider ("new slider"));
    slider20->setRange (0, 10, 0);
    slider20->setSliderStyle (Slider::RotaryVerticalDrag);
    slider20->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider20->addListener (this);

    addAndMakeVisible (label6 = new Label ("new label",
                                           TRANS("Takte (1/16-4)")));
    label6->setFont (Font (15.00f, Font::plain));
    label6->setJustificationType (Justification::centredLeft);
    label6->setEditable (false, false, false);
    label6->setColour (TextEditor::textColourId, Colours::black);
    label6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

RhythmicGateAudioProcessorEditor::~RhythmicGateAudioProcessorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    slider = nullptr;
    slider2 = nullptr;
    slider3 = nullptr;
    slider4 = nullptr;
    slider5 = nullptr;
    slider6 = nullptr;
    slider7 = nullptr;
    slider8 = nullptr;
    slider9 = nullptr;
    slider10 = nullptr;
    slider11 = nullptr;
    slider12 = nullptr;
    slider13 = nullptr;
    slider14 = nullptr;
    slider15 = nullptr;
    slider16 = nullptr;
    slider17 = nullptr;
    slider18 = nullptr;
    slider19 = nullptr;
    label = nullptr;
    label2 = nullptr;
    label3 = nullptr;
    label4 = nullptr;
    label5 = nullptr;
    slider20 = nullptr;
    label6 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void RhythmicGateAudioProcessorEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    g.setColour (Colour (0xffa52a6e));
    g.fillRect (156, 12, 4, 348);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void RhythmicGateAudioProcessorEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    slider->setBounds (8, 8, 136, 32);
    slider2->setBounds (8, 48, 136, 32);
    slider3->setBounds (8, 88, 136, 32);
    slider4->setBounds (8, 128, 136, 32);
    slider5->setBounds (9, 174, 136, 32);
    slider6->setBounds (9, 214, 136, 32);
    slider7->setBounds (9, 254, 136, 32);
    slider8->setBounds (9, 294, 136, 32);
    slider9->setBounds (174, 6, 136, 32);
    slider10->setBounds (174, 46, 136, 32);
    slider11->setBounds (174, 86, 136, 32);
    slider12->setBounds (174, 126, 136, 32);
    slider13->setBounds (170, 174, 136, 32);
    slider14->setBounds (170, 214, 136, 32);
    slider15->setBounds (170, 254, 136, 32);
    slider16->setBounds (170, 294, 136, 32);
    slider17->setBounds (328, 280, 79, 88);
    slider18->setBounds (328, 152, 79, 88);
    slider19->setBounds (328, 24, 79, 88);
    label->setBounds (328, 0, 150, 24);
    label2->setBounds (320, 120, 150, 24);
    label3->setBounds (328, 248, 150, 24);
    label4->setBounds (16, 336, 150, 24);
    label5->setBounds (168, 336, 150, 24);
    slider20->setBounds (328, 416, 79, 88);
    label6->setBounds (320, 384, 150, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void RhythmicGateAudioProcessorEditor::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == slider)
    {
        //[UserSliderCode_slider] -- add your slider handling code here..
        //[/UserSliderCode_slider]
    }
    else if (sliderThatWasMoved == slider2)
    {
        //[UserSliderCode_slider2] -- add your slider handling code here..
        //[/UserSliderCode_slider2]
    }
    else if (sliderThatWasMoved == slider3)
    {
        //[UserSliderCode_slider3] -- add your slider handling code here..
        //[/UserSliderCode_slider3]
    }
    else if (sliderThatWasMoved == slider4)
    {
        //[UserSliderCode_slider4] -- add your slider handling code here..
        //[/UserSliderCode_slider4]
    }
    else if (sliderThatWasMoved == slider5)
    {
        //[UserSliderCode_slider5] -- add your slider handling code here..
        //[/UserSliderCode_slider5]
    }
    else if (sliderThatWasMoved == slider6)
    {
        //[UserSliderCode_slider6] -- add your slider handling code here..
        //[/UserSliderCode_slider6]
    }
    else if (sliderThatWasMoved == slider7)
    {
        //[UserSliderCode_slider7] -- add your slider handling code here..
        //[/UserSliderCode_slider7]
    }
    else if (sliderThatWasMoved == slider8)
    {
        //[UserSliderCode_slider8] -- add your slider handling code here..
        //[/UserSliderCode_slider8]
    }
    else if (sliderThatWasMoved == slider9)
    {
        //[UserSliderCode_slider9] -- add your slider handling code here..
        //[/UserSliderCode_slider9]
    }
    else if (sliderThatWasMoved == slider10)
    {
        //[UserSliderCode_slider10] -- add your slider handling code here..
        //[/UserSliderCode_slider10]
    }
    else if (sliderThatWasMoved == slider11)
    {
        //[UserSliderCode_slider11] -- add your slider handling code here..
        //[/UserSliderCode_slider11]
    }
    else if (sliderThatWasMoved == slider12)
    {
        //[UserSliderCode_slider12] -- add your slider handling code here..
        //[/UserSliderCode_slider12]
    }
    else if (sliderThatWasMoved == slider13)
    {
        //[UserSliderCode_slider13] -- add your slider handling code here..
        //[/UserSliderCode_slider13]
    }
    else if (sliderThatWasMoved == slider14)
    {
        //[UserSliderCode_slider14] -- add your slider handling code here..
        //[/UserSliderCode_slider14]
    }
    else if (sliderThatWasMoved == slider15)
    {
        //[UserSliderCode_slider15] -- add your slider handling code here..
        //[/UserSliderCode_slider15]
    }
    else if (sliderThatWasMoved == slider16)
    {
        //[UserSliderCode_slider16] -- add your slider handling code here..
        //[/UserSliderCode_slider16]
    }
    else if (sliderThatWasMoved == slider17)
    {
        //[UserSliderCode_slider17] -- add your slider handling code here..
        //[/UserSliderCode_slider17]
    }
    else if (sliderThatWasMoved == slider18)
    {
        //[UserSliderCode_slider18] -- add your slider handling code here..
        //[/UserSliderCode_slider18]
    }
    else if (sliderThatWasMoved == slider19)
    {
        //[UserSliderCode_slider19] -- add your slider handling code here..
        //[/UserSliderCode_slider19]
    }
    else if (sliderThatWasMoved == slider20)
    {
        //[UserSliderCode_slider20] -- add your slider handling code here..
        //[/UserSliderCode_slider20]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="RhythmicGateAudioProcessorEditor"
                 componentName="" parentClasses="public Component" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ffffffff">
    <RECT pos="156 12 4 348" fill="solid: ffa52a6e" hasStroke="0"/>
  </BACKGROUND>
  <SLIDER name="new slider" id="1d1c1d4ec1972e9e" memberName="slider" virtualName=""
          explicitFocusOrder="0" pos="8 8 136 32" bkgcol="4ef78181" min="0"
          max="1" int="0.010000000000000000208" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="1ccd840eb1f53483" memberName="slider2"
          virtualName="" explicitFocusOrder="0" pos="8 48 136 32" min="0"
          max="1" int="0.010000000000000000208" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="67ce76a93059cb1c" memberName="slider3"
          virtualName="" explicitFocusOrder="0" pos="8 88 136 32" min="0"
          max="1" int="0.010000000000000000208" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="e8b3fff164a5f0d2" memberName="slider4"
          virtualName="" explicitFocusOrder="0" pos="8 128 136 32" min="0"
          max="1" int="0.010000000000000000208" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="13aeb149a21c775e" memberName="slider5"
          virtualName="" explicitFocusOrder="0" pos="9 174 136 32" bkgcol="4ef78181"
          min="0" max="1" int="0.010000000000000000208" style="LinearBar"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="2873e32a85a9f874" memberName="slider6"
          virtualName="" explicitFocusOrder="0" pos="9 214 136 32" min="0"
          max="1" int="0.010000000000000000208" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="cc1bcbce05608aa3" memberName="slider7"
          virtualName="" explicitFocusOrder="0" pos="9 254 136 32" min="0"
          max="1" int="0.010000000000000000208" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="3882c4a280165b8b" memberName="slider8"
          virtualName="" explicitFocusOrder="0" pos="9 294 136 32" min="0"
          max="1" int="0.010000000000000000208" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="46bf46d432636fa5" memberName="slider9"
          virtualName="" explicitFocusOrder="0" pos="174 6 136 32" bkgcol="4ef78181"
          min="0" max="1" int="0.010000000000000000208" style="LinearBar"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="a13e76a08c854491" memberName="slider10"
          virtualName="" explicitFocusOrder="0" pos="174 46 136 32" min="0"
          max="1" int="0.010000000000000000208" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="db755e884648a7ed" memberName="slider11"
          virtualName="" explicitFocusOrder="0" pos="174 86 136 32" min="0"
          max="1" int="0.010000000000000000208" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="6f790d5656c10ff4" memberName="slider12"
          virtualName="" explicitFocusOrder="0" pos="174 126 136 32" min="0"
          max="1" int="0.010000000000000000208" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="2563b717a584f0aa" memberName="slider13"
          virtualName="" explicitFocusOrder="0" pos="170 174 136 32" bkgcol="4ef78181"
          min="0" max="1" int="0.010000000000000000208" style="LinearBar"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="13a031a77c36dbae" memberName="slider14"
          virtualName="" explicitFocusOrder="0" pos="170 214 136 32" min="0"
          max="1" int="0.010000000000000000208" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="2f718e9aaba00a95" memberName="slider15"
          virtualName="" explicitFocusOrder="0" pos="170 254 136 32" min="0"
          max="1" int="0.010000000000000000208" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="8c1cf09e61e088f7" memberName="slider16"
          virtualName="" explicitFocusOrder="0" pos="170 294 136 32" min="0"
          max="1" int="0.010000000000000000208" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="8d7d9a99121f3b73" memberName="slider17"
          virtualName="" explicitFocusOrder="0" pos="328 280 79 88" min="0"
          max="10" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="c2a689d1d94c28c6" memberName="slider18"
          virtualName="" explicitFocusOrder="0" pos="328 152 79 88" min="0"
          max="10" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="47bc44aed50200ae" memberName="slider19"
          virtualName="" explicitFocusOrder="0" pos="328 24 79 88" min="0"
          max="10" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="66d6ddb589f984bc" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="328 0 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Attack" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="e42ed1603e1e2385" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="320 120 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Release" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="6b3444af6dc11905" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="328 248 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Dry/Wet" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="a65069fc1404e917" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="16 336 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Step 1-8" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="5f426a627fbe795" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="168 336 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Step 9-16" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <SLIDER name="new slider" id="764bed367acc9a99" memberName="slider20"
          virtualName="" explicitFocusOrder="0" pos="328 416 79 88" min="0"
          max="10" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="409ff84439fbc693" memberName="label6" virtualName=""
         explicitFocusOrder="0" pos="320 384 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Takte (1/16-4)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

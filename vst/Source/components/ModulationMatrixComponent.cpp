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

#include "ModulationMatrixComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
ModulationMatrixComponent::ModulationMatrixComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (slider = new Slider ("new slider"));
    slider->setRange (0, 10, 0);
    slider->setSliderStyle (Slider::RotaryVerticalDrag);
    slider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider->addListener (this);

    addAndMakeVisible (comboBox3 = new ComboBox ("new combo box"));
    comboBox3->setEditableText (false);
    comboBox3->setJustificationType (Justification::centredLeft);
    comboBox3->setTextWhenNothingSelected (String::empty);
    comboBox3->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBox3->addListener (this);

    addAndMakeVisible (comboBox2 = new ComboBox ("new combo box"));
    comboBox2->setEditableText (false);
    comboBox2->setJustificationType (Justification::centredLeft);
    comboBox2->setTextWhenNothingSelected (String::empty);
    comboBox2->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBox2->addListener (this);

    addAndMakeVisible (slider2 = new Slider ("new slider"));
    slider2->setRange (0, 10, 0);
    slider2->setSliderStyle (Slider::RotaryVerticalDrag);
    slider2->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider2->addListener (this);

    addAndMakeVisible (comboBox4 = new ComboBox ("new combo box"));
    comboBox4->setEditableText (false);
    comboBox4->setJustificationType (Justification::centredLeft);
    comboBox4->setTextWhenNothingSelected (String::empty);
    comboBox4->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBox4->addListener (this);

    addAndMakeVisible (comboBox5 = new ComboBox ("new combo box"));
    comboBox5->setEditableText (false);
    comboBox5->setJustificationType (Justification::centredLeft);
    comboBox5->setTextWhenNothingSelected (String::empty);
    comboBox5->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBox5->addListener (this);

    addAndMakeVisible (slider3 = new Slider ("new slider"));
    slider3->setRange (0, 10, 0);
    slider3->setSliderStyle (Slider::RotaryVerticalDrag);
    slider3->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider3->addListener (this);

    addAndMakeVisible (comboBox6 = new ComboBox ("new combo box"));
    comboBox6->setEditableText (false);
    comboBox6->setJustificationType (Justification::centredLeft);
    comboBox6->setTextWhenNothingSelected (String::empty);
    comboBox6->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBox6->addListener (this);

    addAndMakeVisible (comboBox7 = new ComboBox ("new combo box"));
    comboBox7->setEditableText (false);
    comboBox7->setJustificationType (Justification::centredLeft);
    comboBox7->setTextWhenNothingSelected (String::empty);
    comboBox7->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBox7->addListener (this);

    addAndMakeVisible (slider4 = new Slider ("new slider"));
    slider4->setRange (0, 10, 0);
    slider4->setSliderStyle (Slider::RotaryVerticalDrag);
    slider4->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider4->addListener (this);

    addAndMakeVisible (comboBox8 = new ComboBox ("new combo box"));
    comboBox8->setEditableText (false);
    comboBox8->setJustificationType (Justification::centredLeft);
    comboBox8->setTextWhenNothingSelected (String::empty);
    comboBox8->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBox8->addListener (this);

    addAndMakeVisible (comboBox9 = new ComboBox ("new combo box"));
    comboBox9->setEditableText (false);
    comboBox9->setJustificationType (Justification::centredLeft);
    comboBox9->setTextWhenNothingSelected (String::empty);
    comboBox9->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBox9->addListener (this);

    addAndMakeVisible (slider5 = new Slider ("new slider"));
    slider5->setRange (0, 10, 0);
    slider5->setSliderStyle (Slider::RotaryVerticalDrag);
    slider5->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider5->addListener (this);

    addAndMakeVisible (comboBox10 = new ComboBox ("new combo box"));
    comboBox10->setEditableText (false);
    comboBox10->setJustificationType (Justification::centredLeft);
    comboBox10->setTextWhenNothingSelected (String::empty);
    comboBox10->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBox10->addListener (this);

    addAndMakeVisible (comboBox11 = new ComboBox ("new combo box"));
    comboBox11->setEditableText (false);
    comboBox11->setJustificationType (Justification::centredLeft);
    comboBox11->setTextWhenNothingSelected (String::empty);
    comboBox11->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBox11->addListener (this);

    addAndMakeVisible (slider6 = new Slider ("new slider"));
    slider6->setRange (0, 10, 0);
    slider6->setSliderStyle (Slider::RotaryVerticalDrag);
    slider6->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider6->addListener (this);

    addAndMakeVisible (comboBox12 = new ComboBox ("new combo box"));
    comboBox12->setEditableText (false);
    comboBox12->setJustificationType (Justification::centredLeft);
    comboBox12->setTextWhenNothingSelected (String::empty);
    comboBox12->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBox12->addListener (this);

    addAndMakeVisible (comboBox13 = new ComboBox ("new combo box"));
    comboBox13->setEditableText (false);
    comboBox13->setJustificationType (Justification::centredLeft);
    comboBox13->setTextWhenNothingSelected (String::empty);
    comboBox13->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBox13->addListener (this);

    addAndMakeVisible (slider7 = new Slider ("new slider"));
    slider7->setRange (0, 10, 0);
    slider7->setSliderStyle (Slider::RotaryVerticalDrag);
    slider7->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider7->addListener (this);

    addAndMakeVisible (comboBox14 = new ComboBox ("new combo box"));
    comboBox14->setEditableText (false);
    comboBox14->setJustificationType (Justification::centredLeft);
    comboBox14->setTextWhenNothingSelected (String::empty);
    comboBox14->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBox14->addListener (this);

    addAndMakeVisible (comboBox15 = new ComboBox ("new combo box"));
    comboBox15->setEditableText (false);
    comboBox15->setJustificationType (Justification::centredLeft);
    comboBox15->setTextWhenNothingSelected (String::empty);
    comboBox15->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBox15->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

ModulationMatrixComponent::~ModulationMatrixComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    slider = nullptr;
    comboBox3 = nullptr;
    comboBox2 = nullptr;
    slider2 = nullptr;
    comboBox4 = nullptr;
    comboBox5 = nullptr;
    slider3 = nullptr;
    comboBox6 = nullptr;
    comboBox7 = nullptr;
    slider4 = nullptr;
    comboBox8 = nullptr;
    comboBox9 = nullptr;
    slider5 = nullptr;
    comboBox10 = nullptr;
    comboBox11 = nullptr;
    slider6 = nullptr;
    comboBox12 = nullptr;
    comboBox13 = nullptr;
    slider7 = nullptr;
    comboBox14 = nullptr;
    comboBox15 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void ModulationMatrixComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff0df8f0));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void ModulationMatrixComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    slider->setBounds (proportionOfWidth (0.0133f), proportionOfHeight (0.0086f), proportionOfWidth (0.2602f), proportionOfHeight (0.1122f));
    comboBox3->setBounds (proportionOfWidth (0.2802f), proportionOfHeight (0.0086f), proportionOfWidth (0.6939f), proportionOfHeight (0.0518f));
    comboBox2->setBounds (proportionOfWidth (0.2802f), proportionOfHeight (0.0690f), proportionOfWidth (0.6939f), proportionOfHeight (0.0518f));
    slider2->setBounds (proportionOfWidth (0.0175f), proportionOfHeight (0.1381f), proportionOfWidth (0.2602f), proportionOfHeight (0.1122f));
    comboBox4->setBounds (proportionOfWidth (0.2836f), proportionOfHeight (0.1381f), proportionOfWidth (0.6939f), proportionOfHeight (0.0518f));
    comboBox5->setBounds (proportionOfWidth (0.2836f), proportionOfHeight (0.1985f), proportionOfWidth (0.6939f), proportionOfHeight (0.0518f));
    slider3->setBounds (proportionOfWidth (0.0192f), proportionOfHeight (0.2632f), proportionOfWidth (0.2602f), proportionOfHeight (0.1122f));
    comboBox6->setBounds (proportionOfWidth (0.2861f), proportionOfHeight (0.2632f), proportionOfWidth (0.6939f), proportionOfHeight (0.0518f));
    comboBox7->setBounds (proportionOfWidth (0.2861f), proportionOfHeight (0.3236f), proportionOfWidth (0.6939f), proportionOfHeight (0.0518f));
    slider4->setBounds (proportionOfWidth (0.0234f), proportionOfHeight (0.3937f), proportionOfWidth (0.2602f), proportionOfHeight (0.1122f));
    comboBox8->setBounds (proportionOfWidth (0.2902f), proportionOfHeight (0.3937f), proportionOfWidth (0.6939f), proportionOfHeight (0.0518f));
    comboBox9->setBounds (proportionOfWidth (0.2902f), proportionOfHeight (0.4552f), proportionOfWidth (0.6939f), proportionOfHeight (0.0518f));
    slider5->setBounds (proportionOfWidth (0.0284f), proportionOfHeight (0.5264f), proportionOfWidth (0.2602f), proportionOfHeight (0.1122f));
    comboBox10->setBounds (proportionOfWidth (0.2953f), proportionOfHeight (0.5264f), proportionOfWidth (0.6939f), proportionOfHeight (0.0518f));
    comboBox11->setBounds (proportionOfWidth (0.2953f), proportionOfHeight (0.5868f), proportionOfWidth (0.6939f), proportionOfHeight (0.0518f));
    slider6->setBounds (proportionOfWidth (0.0309f), proportionOfHeight (0.6537f), proportionOfWidth (0.2602f), proportionOfHeight (0.1122f));
    comboBox12->setBounds (proportionOfWidth (0.2977f), proportionOfHeight (0.6537f), proportionOfWidth (0.6939f), proportionOfHeight (0.0518f));
    comboBox13->setBounds (proportionOfWidth (0.2977f), proportionOfHeight (0.7141f), proportionOfWidth (0.6939f), proportionOfHeight (0.0518f));
    slider7->setBounds (proportionOfWidth (0.0317f), proportionOfHeight (0.7907f), proportionOfWidth (0.2602f), proportionOfHeight (0.1122f));
    comboBox14->setBounds (proportionOfWidth (0.2986f), proportionOfHeight (0.7907f), proportionOfWidth (0.6939f), proportionOfHeight (0.0518f));
    comboBox15->setBounds (proportionOfWidth (0.2986f), proportionOfHeight (0.8522f), proportionOfWidth (0.6939f), proportionOfHeight (0.0518f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void ModulationMatrixComponent::sliderValueChanged (Slider* sliderThatWasMoved)
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

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void ModulationMatrixComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBox3)
    {
        //[UserComboBoxCode_comboBox3] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBox3]
    }
    else if (comboBoxThatHasChanged == comboBox2)
    {
        //[UserComboBoxCode_comboBox2] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBox2]
    }
    else if (comboBoxThatHasChanged == comboBox4)
    {
        //[UserComboBoxCode_comboBox4] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBox4]
    }
    else if (comboBoxThatHasChanged == comboBox5)
    {
        //[UserComboBoxCode_comboBox5] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBox5]
    }
    else if (comboBoxThatHasChanged == comboBox6)
    {
        //[UserComboBoxCode_comboBox6] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBox6]
    }
    else if (comboBoxThatHasChanged == comboBox7)
    {
        //[UserComboBoxCode_comboBox7] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBox7]
    }
    else if (comboBoxThatHasChanged == comboBox8)
    {
        //[UserComboBoxCode_comboBox8] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBox8]
    }
    else if (comboBoxThatHasChanged == comboBox9)
    {
        //[UserComboBoxCode_comboBox9] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBox9]
    }
    else if (comboBoxThatHasChanged == comboBox10)
    {
        //[UserComboBoxCode_comboBox10] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBox10]
    }
    else if (comboBoxThatHasChanged == comboBox11)
    {
        //[UserComboBoxCode_comboBox11] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBox11]
    }
    else if (comboBoxThatHasChanged == comboBox12)
    {
        //[UserComboBoxCode_comboBox12] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBox12]
    }
    else if (comboBoxThatHasChanged == comboBox13)
    {
        //[UserComboBoxCode_comboBox13] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBox13]
    }
    else if (comboBoxThatHasChanged == comboBox14)
    {
        //[UserComboBoxCode_comboBox14] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBox14]
    }
    else if (comboBoxThatHasChanged == comboBox15)
    {
        //[UserComboBoxCode_comboBox15] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBox15]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ModulationMatrixComponent"
                 componentName="" parentClasses="public Component" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff0df8f0"/>
  <SLIDER name="new slider" id="f6f08879a93058f0" memberName="slider" virtualName=""
          explicitFocusOrder="0" pos="1.334% 0.863% 26.022% 11.219%" min="0"
          max="10" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <COMBOBOX name="new combo box" id="b7db82d8a055a4ee" memberName="comboBox3"
            virtualName="" explicitFocusOrder="0" pos="28.023% 0.863% 69.391% 5.178%"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="new combo box" id="646bd6e5692668c8" memberName="comboBox2"
            virtualName="" explicitFocusOrder="0" pos="28.023% 6.904% 69.391% 5.178%"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="new slider" id="3176397a2b7dd6d6" memberName="slider2"
          virtualName="" explicitFocusOrder="0" pos="1.751% 13.808% 26.022% 11.219%"
          min="0" max="10" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <COMBOBOX name="new combo box" id="213f9efddeec291" memberName="comboBox4"
            virtualName="" explicitFocusOrder="0" pos="28.357% 13.808% 69.391% 5.178%"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="new combo box" id="90da7bae1d177065" memberName="comboBox5"
            virtualName="" explicitFocusOrder="0" pos="28.357% 19.849% 69.391% 5.178%"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="new slider" id="e2f06d9246c597b4" memberName="slider3"
          virtualName="" explicitFocusOrder="0" pos="1.918% 26.321% 26.022% 11.219%"
          min="0" max="10" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <COMBOBOX name="new combo box" id="43ca18d9f7da6060" memberName="comboBox6"
            virtualName="" explicitFocusOrder="0" pos="28.607% 26.321% 69.391% 5.178%"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="new combo box" id="b62bd7059ab76ac0" memberName="comboBox7"
            virtualName="" explicitFocusOrder="0" pos="28.607% 32.362% 69.391% 5.178%"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="new slider" id="ca418ad06b5b8910" memberName="slider4"
          virtualName="" explicitFocusOrder="0" pos="2.335% 39.374% 26.022% 11.219%"
          min="0" max="10" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <COMBOBOX name="new combo box" id="a688b9eb93e0b151" memberName="comboBox8"
            virtualName="" explicitFocusOrder="0" pos="29.024% 39.374% 69.391% 5.178%"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="new combo box" id="ee9686c3d0c11b09" memberName="comboBox9"
            virtualName="" explicitFocusOrder="0" pos="29.024% 45.523% 69.391% 5.178%"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="new slider" id="46be721db4317a81" memberName="slider5"
          virtualName="" explicitFocusOrder="0" pos="2.836% 52.643% 26.022% 11.219%"
          min="0" max="10" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <COMBOBOX name="new combo box" id="22752d9a3de7b204" memberName="comboBox10"
            virtualName="" explicitFocusOrder="0" pos="29.525% 52.643% 69.391% 5.178%"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="new combo box" id="fcf91e3060635091" memberName="comboBox11"
            virtualName="" explicitFocusOrder="0" pos="29.525% 58.684% 69.391% 5.178%"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="new slider" id="774ed45b9613caf" memberName="slider6" virtualName=""
          explicitFocusOrder="0" pos="3.086% 65.372% 26.022% 11.219%" min="0"
          max="10" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <COMBOBOX name="new combo box" id="412531e0dec8984b" memberName="comboBox12"
            virtualName="" explicitFocusOrder="0" pos="29.775% 65.372% 69.391% 5.178%"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="new combo box" id="911643bc8e4b58fb" memberName="comboBox13"
            virtualName="" explicitFocusOrder="0" pos="29.775% 71.413% 69.391% 5.178%"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="new slider" id="1fe26777a1bc2fd8" memberName="slider7"
          virtualName="" explicitFocusOrder="0" pos="3.169% 79.072% 26.022% 11.219%"
          min="0" max="10" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <COMBOBOX name="new combo box" id="efc59eb9fbc245ad" memberName="comboBox14"
            virtualName="" explicitFocusOrder="0" pos="29.858% 79.072% 69.391% 5.178%"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="new combo box" id="ac2e7179b10ef6be" memberName="comboBox15"
            virtualName="" explicitFocusOrder="0" pos="29.858% 85.221% 69.391% 5.178%"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

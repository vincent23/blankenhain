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

#include "WaveVerschwurbelungComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
WaveVerschwurbelungComponent::WaveVerschwurbelungComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (comboBox = new ComboBox ("new combo box"));
    comboBox->setEditableText (false);
    comboBox->setJustificationType (Justification::centredLeft);
    comboBox->setTextWhenNothingSelected (String::empty);
    comboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBox->addItem (TRANS("Add"), 1);
    comboBox->addListener (this);

    addAndMakeVisible (slider = new Slider ("new slider"));
    slider->setRange (0, 10, 0);
    slider->setSliderStyle (Slider::RotaryVerticalDrag);
    slider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider->addListener (this);

    addAndMakeVisible (slider2 = new Slider ("new slider"));
    slider2->setRange (0, 10, 0);
    slider2->setSliderStyle (Slider::RotaryVerticalDrag);
    slider2->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider2->addListener (this);

    addAndMakeVisible (label = new Label ("new label",
                                          TRANS("Mix")));
    label->setFont (Font (15.00f, Font::plain));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label2 = new Label ("new label",
                                           TRANS("Phase Offset")));
    label2->setFont (Font (15.00f, Font::plain));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (component = new Component());
    component->setName ("new component");


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

WaveVerschwurbelungComponent::~WaveVerschwurbelungComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    comboBox = nullptr;
    slider = nullptr;
    slider2 = nullptr;
    label = nullptr;
    label2 = nullptr;
    component = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void WaveVerschwurbelungComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xfffb3bdf));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void WaveVerschwurbelungComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    comboBox->setBounds (proportionOfWidth (0.0225f), proportionOfHeight (0.0433f), proportionOfWidth (0.9564f), proportionOfHeight (0.0433f));
    slider->setBounds (proportionOfWidth (0.0338f), proportionOfHeight (0.1588f), proportionOfWidth (0.4613f), proportionOfHeight (0.5199f));
    slider2->setBounds (proportionOfWidth (0.5288f), proportionOfHeight (0.1588f), proportionOfWidth (0.4501f), proportionOfHeight (0.5199f));
    label->setBounds (proportionOfWidth (0.0450f), proportionOfHeight (0.1011f), proportionOfWidth (0.4163f), proportionOfHeight (0.0433f));
    label2->setBounds (proportionOfWidth (0.5401f), proportionOfHeight (0.1011f), proportionOfWidth (0.4163f), proportionOfHeight (0.0433f));
    component->setBounds (proportionOfWidth (0.0225f), proportionOfHeight (0.6931f), proportionOfWidth (0.9564f), proportionOfHeight (0.2888f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void WaveVerschwurbelungComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBox)
    {
        //[UserComboBoxCode_comboBox] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void WaveVerschwurbelungComponent::sliderValueChanged (Slider* sliderThatWasMoved)
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

<JUCER_COMPONENT documentType="Component" className="WaveVerschwurbelungComponent"
                 componentName="" parentClasses="public Component" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="fffb3bdf"/>
  <COMBOBOX name="new combo box" id="f6ea8388bdd58be" memberName="comboBox"
            virtualName="" explicitFocusOrder="0" pos="2.25% 4.332% 95.64% 4.332%"
            editable="0" layout="33" items="Add" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="new slider" id="171066cb6dea4672" memberName="slider" virtualName=""
          explicitFocusOrder="0" pos="3.376% 15.884% 46.132% 51.986%" min="0"
          max="10" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="14d529117a9423a7" memberName="slider2"
          virtualName="" explicitFocusOrder="0" pos="52.883% 15.884% 45.007% 51.986%"
          min="0" max="10" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="ee1a6bed0846c86b" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="4.501% 10.108% 41.632% 4.332%" edTextCol="ff000000"
         edBkgCol="0" labelText="Mix" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="e767c9cb9fd478a8" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="54.008% 10.108% 41.632% 4.332%" edTextCol="ff000000"
         edBkgCol="0" labelText="Phase Offset" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <GENERICCOMPONENT name="new component" id="736dede34e64bd88" memberName="component"
                    virtualName="" explicitFocusOrder="0" pos="2.25% 69.314% 95.64% 28.881%"
                    class="Component" params=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

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

#include "LfoComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
LfoComponent::LfoComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (comboBox = new ComboBox ("new combo box"));
    comboBox->setEditableText (false);
    comboBox->setJustificationType (Justification::centredLeft);
    comboBox->setTextWhenNothingSelected (String::empty);
    comboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBox->addItem (TRANS("Sine"), 1);
    comboBox->addItem (TRANS("Square"), 2);
    comboBox->addItem (TRANS("Saw"), 3);
    comboBox->addItem (TRANS("Triangle"), 4);
    comboBox->addItem (TRANS("Noise"), 5);
    comboBox->addListener (this);

    addAndMakeVisible (slider = new Slider ("new slider"));
    slider->setRange (0, 10, 0);
    slider->setSliderStyle (Slider::RotaryVerticalDrag);
    slider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider->addListener (this);

    addAndMakeVisible (label = new Label ("new label",
                                          TRANS("Rate")));
    label->setFont (Font (15.00f, Font::plain));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggleButton = new ToggleButton ("new toggle button"));
    toggleButton->setButtonText (TRANS("Sync"));
    toggleButton->addListener (this);

    addAndMakeVisible (toggleButton2 = new ToggleButton ("new toggle button"));
    toggleButton2->setButtonText (TRANS("Retrigger"));
    toggleButton2->addListener (this);

    addAndMakeVisible (label2 = new Label ("new label",
                                           TRANS("Phase")));
    label2->setFont (Font (15.00f, Font::plain));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (component = new Component());
    component->setName ("new component");

    addAndMakeVisible (slider2 = new Slider ("new slider"));
    slider2->setRange (0, 10, 0);
    slider2->setSliderStyle (Slider::RotaryVerticalDrag);
    slider2->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider2->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

LfoComponent::~LfoComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    comboBox = nullptr;
    slider = nullptr;
    label = nullptr;
    toggleButton = nullptr;
    toggleButton2 = nullptr;
    label2 = nullptr;
    component = nullptr;
    slider2 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void LfoComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffdc6212));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void LfoComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    comboBox->setBounds (proportionOfWidth (0.3235f), proportionOfHeight (0.0679f), proportionOfWidth (0.2897f), proportionOfHeight (0.0679f));
    slider->setBounds (proportionOfWidth (0.0132f), proportionOfHeight (0.1877f), proportionOfWidth (0.2700f), proportionOfHeight (0.4677f));
    label->setBounds (proportionOfWidth (0.0403f), proportionOfHeight (0.0498f), proportionOfWidth (0.2634f), proportionOfHeight (0.1103f));
    toggleButton->setBounds (proportionOfWidth (0.3235f), proportionOfHeight (0.2450f), proportionOfWidth (0.2963f), proportionOfHeight (0.1198f));
    toggleButton2->setBounds (proportionOfWidth (0.3300f), proportionOfHeight (0.4401f), proportionOfWidth (0.2831f), proportionOfHeight (0.1347f));
    label2->setBounds (proportionOfWidth (0.6716f), proportionOfHeight (0.0255f), proportionOfWidth (0.2963f), proportionOfHeight (0.1347f));
    component->setBounds (proportionOfWidth (0.0132f), proportionOfHeight (0.6702f), proportionOfWidth (0.9753f), proportionOfHeight (0.3224f));
    slider2->setBounds (proportionOfWidth (0.6387f), proportionOfHeight (0.1782f), proportionOfWidth (0.3226f), proportionOfHeight (0.4242f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void LfoComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
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

void LfoComponent::sliderValueChanged (Slider* sliderThatWasMoved)
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

void LfoComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == toggleButton)
    {
        //[UserButtonCode_toggleButton] -- add your button handler code here..
        //[/UserButtonCode_toggleButton]
    }
    else if (buttonThatWasClicked == toggleButton2)
    {
        //[UserButtonCode_toggleButton2] -- add your button handler code here..
        //[/UserButtonCode_toggleButton2]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="LfoComponent" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ffdc6212"/>
  <COMBOBOX name="new combo box" id="ba8fae1acff7a4a" memberName="comboBox"
            virtualName="" explicitFocusOrder="0" pos="32.346% 6.787% 28.971% 6.787%"
            editable="0" layout="33" items="Sine&#10;Square&#10;Saw&#10;Triangle&#10;Noise"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="new slider" id="9f224d2ae4bea60" memberName="slider" virtualName=""
          explicitFocusOrder="0" pos="1.317% 18.77% 26.996% 46.766%" min="0"
          max="10" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="55cd6676fd46efcf" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="4.033% 4.984% 26.337% 11.029%" edTextCol="ff000000"
         edBkgCol="0" labelText="Rate" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="new toggle button" id="b7df8e4e43b6191f" memberName="toggleButton"
                virtualName="" explicitFocusOrder="0" pos="32.346% 24.496% 29.63% 11.983%"
                buttonText="Sync" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="6402c11d163a4e20" memberName="toggleButton2"
                virtualName="" explicitFocusOrder="0" pos="33.004% 44.008% 28.313% 13.468%"
                buttonText="Retrigger" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <LABEL name="new label" id="a095842aaf6812be" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="67.16% 2.545% 29.63% 13.468%" edTextCol="ff000000"
         edBkgCol="0" labelText="Phase" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <GENERICCOMPONENT name="new component" id="1c9390e70f3afc0a" memberName="component"
                    virtualName="" explicitFocusOrder="0" pos="1.317% 67.02% 97.531% 32.238%"
                    class="Component" params=""/>
  <SLIDER name="new slider" id="5f9514bcb6523431" memberName="slider2"
          virtualName="" explicitFocusOrder="0" pos="63.868% 17.815% 32.263% 42.418%"
          min="0" max="10" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

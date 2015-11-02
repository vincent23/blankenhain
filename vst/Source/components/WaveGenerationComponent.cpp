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

#include "WaveGenerationComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
WaveGenerationComponent::WaveGenerationComponent ()
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
    slider->setSliderStyle (Slider::IncDecButtons);
    slider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider->addListener (this);

    addAndMakeVisible (slider2 = new Slider ("new slider"));
    slider2->setRange (0, 10, 0);
    slider2->setSliderStyle (Slider::IncDecButtons);
    slider2->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider2->addListener (this);

    addAndMakeVisible (slider3 = new Slider ("new slider"));
    slider3->setRange (0, 10, 0);
    slider3->setSliderStyle (Slider::RotaryVerticalDrag);
    slider3->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    slider3->addListener (this);

    addAndMakeVisible (label = new Label ("new label",
                                          TRANS("Octave")));
    label->setFont (Font (15.00f, Font::plain));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label2 = new Label ("new label",
                                           TRANS("Pitch")));
    label2->setFont (Font (15.00f, Font::plain));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label3 = new Label ("new label",
                                           TRANS("Detune")));
    label3->setFont (Font (15.00f, Font::plain));
    label3->setJustificationType (Justification::centred);
    label3->setEditable (false, false, false);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

WaveGenerationComponent::~WaveGenerationComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    comboBox = nullptr;
    slider = nullptr;
    slider2 = nullptr;
    slider3 = nullptr;
    label = nullptr;
    label2 = nullptr;
    label3 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void WaveGenerationComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffe22828));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void WaveGenerationComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    comboBox->setBounds (proportionOfWidth (0.0225f), proportionOfHeight (0.0207f), proportionOfWidth (0.9677f), proportionOfHeight (0.1034f));
    slider->setBounds (proportionOfWidth (0.0338f), proportionOfHeight (0.2687f), proportionOfWidth (0.5513f), proportionOfHeight (0.2894f));
    slider2->setBounds (proportionOfWidth (0.0338f), proportionOfHeight (0.7028f), proportionOfWidth (0.5738f), proportionOfHeight (0.2687f));
    slider3->setBounds (proportionOfWidth (0.6526f), proportionOfHeight (0.2894f), proportionOfWidth (0.3263f), proportionOfHeight (0.6822f));
    label->setBounds (proportionOfWidth (0.0338f), proportionOfHeight (0.1447f), proportionOfWidth (0.5288f), proportionOfHeight (0.1240f));
    label2->setBounds (proportionOfWidth (0.0338f), proportionOfHeight (0.5788f), proportionOfWidth (0.5738f), proportionOfHeight (0.1034f));
    label3->setBounds (proportionOfWidth (0.6301f), proportionOfHeight (0.1447f), proportionOfWidth (0.3488f), proportionOfHeight (0.1240f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void WaveGenerationComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
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

void WaveGenerationComponent::sliderValueChanged (Slider* sliderThatWasMoved)
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

<JUCER_COMPONENT documentType="Component" className="WaveGenerationComponent"
                 componentName="" parentClasses="public Component" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ffe22828"/>
  <COMBOBOX name="new combo box" id="ca96b1237e69bbe9" memberName="comboBox"
            virtualName="" explicitFocusOrder="0" pos="2.25% 2.067% 96.765% 10.336%"
            editable="0" layout="33" items="Sine&#10;Square&#10;Saw&#10;Triangle&#10;Noise"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="new slider" id="dc401c724a6ad0b4" memberName="slider" virtualName=""
          explicitFocusOrder="0" pos="3.376% 26.873% 55.134% 28.941%" min="0"
          max="10" int="0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="7654c27a6cbb394f" memberName="slider2"
          virtualName="" explicitFocusOrder="0" pos="3.376% 70.284% 57.384% 26.873%"
          min="0" max="10" int="0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="e7858666d3b4470d" memberName="slider3"
          virtualName="" explicitFocusOrder="0" pos="65.26% 28.941% 32.63% 68.217%"
          min="0" max="10" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="59071bf6c3ad4b05" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="3.376% 14.47% 52.883% 12.403%" edTextCol="ff000000"
         edBkgCol="0" labelText="Octave" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="7f5b586cb34be50" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="3.376% 57.881% 57.384% 10.336%" edTextCol="ff000000"
         edBkgCol="0" labelText="Pitch" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="4999af7f56e76ee5" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="63.01% 14.47% 34.88% 12.403%" edTextCol="ff000000"
         edBkgCol="0" labelText="Detune" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

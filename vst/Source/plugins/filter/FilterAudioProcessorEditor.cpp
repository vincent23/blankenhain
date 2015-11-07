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

#include "FilterAudioProcessorEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
FilterAudioProcessorEditor::FilterAudioProcessorEditor (FilterAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (label = new Label ("new label",
                                          TRANS("LFO")));
    label->setFont (Font (100.00f, Font::plain));
    label->setJustificationType (Justification::centred);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (filterTypeSelector = new ComboBox ("new combo box"));
    filterTypeSelector->setEditableText (false);
    filterTypeSelector->setJustificationType (Justification::centredLeft);
    filterTypeSelector->setTextWhenNothingSelected (TRANS("filter type"));
    filterTypeSelector->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    filterTypeSelector->addItem (TRANS("High"), 1);
    filterTypeSelector->addItem (TRANS("Low"), 2);
    filterTypeSelector->addItem (TRANS("Band"), 3);
    filterTypeSelector->addItem (TRANS("Notch"), 4);
    filterTypeSelector->addListener (this);

    addAndMakeVisible (frequencySlider = new Slider ("new slider"));
    frequencySlider->setRange (40, 22000, 1);
    frequencySlider->setSliderStyle (Slider::RotaryVerticalDrag);
    frequencySlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    frequencySlider->addListener (this);
    frequencySlider->setSkewFactor (0.3);

    addAndMakeVisible (resonanceSlider = new Slider ("new slider"));
    resonanceSlider->setRange (0.1, 5, 0.01);
    resonanceSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    resonanceSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    resonanceSlider->addListener (this);

    addAndMakeVisible (label2 = new Label ("new label",
                                           TRANS("Resonance (Q)")));
    label2->setFont (Font (15.00f, Font::plain));
    label2->setJustificationType (Justification::centred);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label3 = new Label ("new label",
                                           TRANS("Frequency")));
    label3->setFont (Font (15.00f, Font::plain));
    label3->setJustificationType (Justification::centred);
    label3->setEditable (false, false, false);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (500, 300);


    //[Constructor] You can add your own custom stuff here..
	startTimer(100);
    //[/Constructor]
}

FilterAudioProcessorEditor::~FilterAudioProcessorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    label = nullptr;
    filterTypeSelector = nullptr;
    frequencySlider = nullptr;
    resonanceSlider = nullptr;
    label2 = nullptr;
    label3 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void FilterAudioProcessorEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void FilterAudioProcessorEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    label->setBounds (proportionOfWidth (0.6995f), proportionOfHeight (0.0000f), proportionOfWidth (0.3005f), proportionOfHeight (1.0000f));
    filterTypeSelector->setBounds (proportionOfWidth (0.0095f), proportionOfHeight (0.0095f), proportionOfWidth (0.6995f), proportionOfHeight (0.0792f));
    frequencySlider->setBounds (proportionOfWidth (0.0095f), proportionOfHeight (0.0998f), proportionOfWidth (0.3397f), proportionOfHeight (0.8003f));
    resonanceSlider->setBounds (proportionOfWidth (0.3599f), proportionOfHeight (0.0998f), proportionOfWidth (0.3397f), proportionOfHeight (0.8003f));
    label2->setBounds (proportionOfWidth (0.3599f), proportionOfHeight (0.9002f), proportionOfWidth (0.3504f), proportionOfHeight (0.0998f));
    label3->setBounds (proportionOfWidth (0.0095f), proportionOfHeight (0.9002f), proportionOfWidth (0.3504f), proportionOfHeight (0.0998f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void FilterAudioProcessorEditor::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == filterTypeSelector)
    {
        //[UserComboBoxCode_filterTypeSelector] -- add your combo box handling code here..
		processor.setFilterType(comboBoxThatHasChanged->getSelectedItemIndex());
        //[/UserComboBoxCode_filterTypeSelector]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void FilterAudioProcessorEditor::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == frequencySlider)
    {
        //[UserSliderCode_frequencySlider] -- add your slider handling code here..
		processor.setFrequency(sliderThatWasMoved->getValue());
        //[/UserSliderCode_frequencySlider]
    }
    else if (sliderThatWasMoved == resonanceSlider)
    {
        //[UserSliderCode_resonanceSlider] -- add your slider handling code here..
		processor.setResonance(sliderThatWasMoved->getValue());
        //[/UserSliderCode_resonanceSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void FilterAudioProcessorEditor::timerCallback()
{
	resonanceSlider->setValue(processor.getResonance(), juce::dontSendNotification);
	frequencySlider->setValue(processor.getFrequency(), juce::dontSendNotification);
	filterTypeSelector->setSelectedItemIndex(processor.getFilterType(), juce::dontSendNotification);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="FilterAudioProcessorEditor"
                 componentName="" parentClasses="public AudioProcessorEditor, public Timer"
                 constructorParams="FilterAudioProcessor&amp; p" variableInitialisers="AudioProcessorEditor(&amp;p), processor(p)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="500" initialHeight="300">
  <BACKGROUND backgroundColour="ffffffff"/>
  <LABEL name="new label" id="6f0b03fe092fc1cc" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="69.952% 0% 30.048% 100%" edTextCol="ff000000"
         edBkgCol="0" labelText="LFO" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="100"
         bold="0" italic="0" justification="36"/>
  <COMBOBOX name="new combo box" id="1dcc59c629cf3d96" memberName="filterTypeSelector"
            virtualName="" explicitFocusOrder="0" pos="0.95% 0.951% 69.952% 7.924%"
            editable="0" layout="33" items="High&#10;Low&#10;Band&#10;Notch"
            textWhenNonSelected="filter type" textWhenNoItems="(no choices)"/>
  <SLIDER name="new slider" id="8ccfb48a68fba8e0" memberName="frequencySlider"
          virtualName="" explicitFocusOrder="0" pos="0.95% 9.984% 33.967% 80.032%"
          min="40" max="22000" int="1" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="0.2999999999999999889"/>
  <SLIDER name="new slider" id="e475b1e27e22f86a" memberName="resonanceSlider"
          virtualName="" explicitFocusOrder="0" pos="35.986% 9.984% 33.967% 80.032%"
          min="0.10000000000000000555" max="5" int="0.010000000000000000208"
          style="RotaryVerticalDrag" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="7e1f26a2d2304b85" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="35.986% 90.016% 35.036% 9.984%" edTextCol="ff000000"
         edBkgCol="0" labelText="Resonance (Q)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="2d2e1e1a6e121059" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="0.95% 90.016% 35.036% 9.984%" edTextCol="ff000000"
         edBkgCol="0" labelText="Frequency" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

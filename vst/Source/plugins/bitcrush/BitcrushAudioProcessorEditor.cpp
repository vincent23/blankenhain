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

#include "BitcrushAudioProcessorEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
BitcrushAudioProcessorEditor::BitcrushAudioProcessorEditor (BitcrushAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (bitcrushSlider = new Slider ("new slider"));
    bitcrushSlider->setRange (0, 1, 0.01);
    bitcrushSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    bitcrushSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    bitcrushSlider->addListener (this);

    addAndMakeVisible (label = new Label ("new label",
                                          TRANS("Bitcrush")));
    label->setFont (Font (15.00f, Font::plain));
    label->setJustificationType (Justification::centred);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (downsampleSlider = new Slider ("new slider"));
    downsampleSlider->setRange (0, 1, 0.01);
    downsampleSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    downsampleSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    downsampleSlider->addListener (this);

    addAndMakeVisible (label2 = new Label ("new label",
                                           TRANS("Downsample")));
    label2->setFont (Font (15.00f, Font::plain));
    label2->setJustificationType (Justification::centred);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (wetSlider = new Slider ("new slider"));
    wetSlider->setRange (0, 1, 0.01);
    wetSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    wetSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    wetSlider->addListener (this);

    addAndMakeVisible (label3 = new Label ("new label",
                                           TRANS("Dry/Wet")));
    label3->setFont (Font (15.00f, Font::plain));
    label3->setJustificationType (Justification::centred);
    label3->setEditable (false, false, false);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
	startTimer(100);
    //[/Constructor]
}

BitcrushAudioProcessorEditor::~BitcrushAudioProcessorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    bitcrushSlider = nullptr;
    label = nullptr;
    downsampleSlider = nullptr;
    label2 = nullptr;
    wetSlider = nullptr;
    label3 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void BitcrushAudioProcessorEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void BitcrushAudioProcessorEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    bitcrushSlider->setBounds (32, 152, 150, 128);
    label->setBounds (32, 120, 150, 24);
    downsampleSlider->setBounds (228, 156, 150, 128);
    label2->setBounds (228, 124, 150, 24);
    wetSlider->setBounds (411, 154, 150, 128);
    label3->setBounds (411, 122, 150, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void BitcrushAudioProcessorEditor::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == bitcrushSlider)
    {
        //[UserSliderCode_bitcrushSlider] -- add your slider handling code here..
		processor.setBitcrush(bitcrushSlider->getValue());
        //[/UserSliderCode_bitcrushSlider]
    }
    else if (sliderThatWasMoved == downsampleSlider)
    {
        //[UserSliderCode_downsampleSlider] -- add your slider handling code here..
		processor.setDownsample(downsampleSlider->getValue());
        //[/UserSliderCode_downsampleSlider]
    }
    else if (sliderThatWasMoved == wetSlider)
    {
        //[UserSliderCode_wetSlider] -- add your slider handling code here..
		processor.setWet(wetSlider->getValue());
        //[/UserSliderCode_wetSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void BitcrushAudioProcessorEditor::timerCallback()
{
	bitcrushSlider->setValue(processor.getBitcrush(), juce::dontSendNotification);
	downsampleSlider->setValue(processor.getDownsample(), juce::dontSendNotification);
	wetSlider->setValue(processor.getWet(), juce::dontSendNotification);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="BitcrushAudioProcessorEditor"
                 componentName="" parentClasses="public AudioProcessorEditor, public Timer"
                 constructorParams="BitcrushAudioProcessor&amp; p" variableInitialisers="AudioProcessorEditor(&amp;p), processor(p)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ffffffff"/>
  <SLIDER name="new slider" id="b5861bd8e286163" memberName="bitcrushSlider"
          virtualName="" explicitFocusOrder="0" pos="32 152 150 128" min="0"
          max="1" int="0.010000000000000000208" style="RotaryVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="200d0a1d62b4a5cf" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="32 120 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Bitcrush" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <SLIDER name="new slider" id="6167eff95746422b" memberName="downsampleSlider"
          virtualName="" explicitFocusOrder="0" pos="228 156 150 128" min="0"
          max="1" int="0.010000000000000000208" style="RotaryVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="faa41b91f6352928" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="228 124 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Downsample" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <SLIDER name="new slider" id="1f984711f42a8c25" memberName="wetSlider"
          virtualName="" explicitFocusOrder="0" pos="411 154 150 128" min="0"
          max="1" int="0.010000000000000000208" style="RotaryVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="ed75009d2f079042" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="411 122 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Dry/Wet" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

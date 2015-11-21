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

#include "BitcrushAudioProcessorEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
BitcrushAudioProcessorEditor::BitcrushAudioProcessorEditor (BitcrushAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (label2 = new Label ("new label",
                                           TRANS("Downsample")));
    label2->setFont (Font (15.00f, Font::plain));
    label2->setJustificationType (Justification::centred);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

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

    addAndMakeVisible (meterChild = new MeterComponent());

    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 300);


    //[Constructor] You can add your own custom stuff here..
	startTimer(100);
    //[/Constructor]
}

BitcrushAudioProcessorEditor::~BitcrushAudioProcessorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    label2 = nullptr;
    bitcrushSlider = nullptr;
    label = nullptr;
    downsampleSlider = nullptr;
    wetSlider = nullptr;
    label3 = nullptr;
    meterChild = nullptr;


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

    label2->setBounds (168, 80, 104, 24);
    bitcrushSlider->setBounds (24, 120, 112, 120);
    label->setBounds (24, 88, 112, 24);
    downsampleSlider->setBounds (168, 112, 104, 124);
    wetSlider->setBounds (296, 104, 112, 128);
    label3->setBounds (304, 80, 104, 24);
    meterChild->setBounds (424, 0, 258, 352);
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

	// Do not worry about a thing and let this neat function
	// do all the paint/repaint stuff regarding metering.
	// Pass stuff on like this ;)
	meterChild->setValue(processor.getMeterValues(), processor.getLastKnownSampleRate(), processor.getLastKnownBlockSize());
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
                 fixedSize="1" initialWidth="600" initialHeight="300">
  <BACKGROUND backgroundColour="ffffffff"/>
  <LABEL name="new label" id="faa41b91f6352928" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="168 80 104 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Downsample" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <SLIDER name="new slider" id="b5861bd8e286163" memberName="bitcrushSlider"
          virtualName="" explicitFocusOrder="0" pos="24 120 112 120" min="0"
          max="1" int="0.010000000000000000208" style="RotaryVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="200d0a1d62b4a5cf" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="24 88 112 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Bitcrush" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <SLIDER name="new slider" id="6167eff95746422b" memberName="downsampleSlider"
          virtualName="" explicitFocusOrder="0" pos="168 112 104 124" min="0"
          max="1" int="0.010000000000000000208" style="RotaryVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="1f984711f42a8c25" memberName="wetSlider"
          virtualName="" explicitFocusOrder="0" pos="296 104 112 128" min="0"
          max="1" int="0.010000000000000000208" style="RotaryVerticalDrag"
          textBoxPos="TextBoxBelow" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="ed75009d2f079042" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="304 80 104 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Dry/Wet" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <JUCERCOMP name="meterChild" id="24679acdf902a533" memberName="meterChild"
             virtualName="" explicitFocusOrder="0" pos="424 0 258 352" sourceFile="../../components/MeterComponent.cpp"
             constructorParams=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

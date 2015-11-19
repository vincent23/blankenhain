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

#include "VolumeAudioProcessorEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VolumeAudioProcessorEditor::VolumeAudioProcessorEditor (VolumeAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (volumeRSlider = new Slider ("volumeRSlider"));
    volumeRSlider->setTooltip (TRANS("controls right channel volume"));
    volumeRSlider->setRange (-120, 12, 0.1);
    volumeRSlider->setSliderStyle (Slider::LinearVertical);
    volumeRSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    volumeRSlider->setColour (Slider::backgroundColourId, Colour (0x75ffffff));
    volumeRSlider->addListener (this);
    volumeRSlider->setSkewFactor (5);

    addAndMakeVisible (volumeLSlider = new Slider ("volumeLSlider"));
    volumeLSlider->setTooltip (TRANS("Controls left channel volume"));
    volumeLSlider->setRange (-120, 12, 0.1);
    volumeLSlider->setSliderStyle (Slider::LinearVertical);
    volumeLSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    volumeLSlider->setColour (Slider::backgroundColourId, Colour (0x75ffffff));
    volumeLSlider->addListener (this);
    volumeLSlider->setSkewFactor (5);

    addAndMakeVisible (stereoCouplingButton = new ToggleButton ("stereoCouplingButton"));
    stereoCouplingButton->setButtonText (TRANS("Stereo Coupling"));
    stereoCouplingButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnBottom);
    stereoCouplingButton->addListener (this);
    stereoCouplingButton->setToggleState (true, dontSendNotification);
    stereoCouplingButton->setColour (ToggleButton::textColourId, Colours::black);

    addAndMakeVisible (label = new Label ("new label",
                                          TRANS("Left Channel")));
    label->setFont (Font ("Arial", 15.00f, Font::plain));
    label->setJustificationType (Justification::centred);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label2 = new Label ("new label",
                                           TRANS("Right Channel")));
    label2->setFont (Font ("Arial", 15.00f, Font::plain));
    label2->setJustificationType (Justification::centred);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (meterChild = new MeterComponent());

    //[UserPreSize]
    //[/UserPreSize]

    setSize (500, 200);


    //[Constructor] You can add your own custom stuff here..
	startTimer(100);
	volumeRSlider->setEnabled(false);
    //[/Constructor]
}

VolumeAudioProcessorEditor::~VolumeAudioProcessorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    volumeRSlider = nullptr;
    volumeLSlider = nullptr;
    stereoCouplingButton = nullptr;
    label = nullptr;
    label2 = nullptr;
    meterChild = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VolumeAudioProcessorEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void VolumeAudioProcessorEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    volumeRSlider->setBounds (100, 0, 100, 150);
    volumeLSlider->setBounds (0, 0, 100, 150);
    stereoCouplingButton->setBounds (0, 170, 200, 30);
    label->setBounds (0, 150, 100, 20);
    label2->setBounds (100, 150, 100, 20);
    meterChild->setBounds (200, 0, 300, 200);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void VolumeAudioProcessorEditor::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == volumeRSlider)
    {
        //[UserSliderCode_volumeRSlider] -- add your slider handling code here..
		processor.setVolumeR(sliderThatWasMoved->getValue());
        //[/UserSliderCode_volumeRSlider]
    }
    else if (sliderThatWasMoved == volumeLSlider)
    {
        //[UserSliderCode_volumeLSlider] -- add your slider handling code here..
		processor.setVolumeL(sliderThatWasMoved->getValue());
		if (processor.getStereoCoupling())
		{
			volumeRSlider->setValue(sliderThatWasMoved->getValue());
			processor.setVolumeR(sliderThatWasMoved->getValue());
		}

        //[/UserSliderCode_volumeLSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void VolumeAudioProcessorEditor::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == stereoCouplingButton)
    {
        //[UserButtonCode_stereoCouplingButton] -- add your button handler code here..
		processor.switchStereoCoupling();
		if (processor.getStereoCoupling())
		{
			volumeRSlider->setEnabled(false);
			volumeRSlider->setValue(volumeLSlider->getValue());
		}
		else
		{
			volumeRSlider->setEnabled(true);
			volumeRSlider->setValue(volumeLSlider->getValue());
		}
        //[/UserButtonCode_stereoCouplingButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void VolumeAudioProcessorEditor::timerCallback()
{
	volumeRSlider->setValue(processor.getVolumeR(), juce::dontSendNotification);
	volumeLSlider->setValue(processor.getVolumeL(), juce::dontSendNotification);
	stereoCouplingButton->setToggleState(processor.getStereoCoupling(), juce::dontSendNotification);

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

<JUCER_COMPONENT documentType="Component" className="VolumeAudioProcessorEditor"
                 componentName="" parentClasses="public AudioProcessorEditor, public Timer"
                 constructorParams="VolumeAudioProcessor&amp; p" variableInitialisers="AudioProcessorEditor(&amp;p), processor(p)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="500" initialHeight="200">
  <BACKGROUND backgroundColour="ffffffff"/>
  <SLIDER name="volumeRSlider" id="cdb02c40117b4d0c" memberName="volumeRSlider"
          virtualName="" explicitFocusOrder="0" pos="100 0 100 150" tooltip="controls right channel volume"
          bkgcol="75ffffff" min="-120" max="12" int="0.10000000000000000555"
          style="LinearVertical" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="5"/>
  <SLIDER name="volumeLSlider" id="821d9281dc888e9a" memberName="volumeLSlider"
          virtualName="" explicitFocusOrder="0" pos="0 0 100 150" tooltip="Controls left channel volume"
          bkgcol="75ffffff" min="-120" max="12" int="0.10000000000000000555"
          style="LinearVertical" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="5"/>
  <TOGGLEBUTTON name="stereoCouplingButton" id="40b98681bdbd4219" memberName="stereoCouplingButton"
                virtualName="" explicitFocusOrder="0" pos="0 170 200 30" txtcol="ff000000"
                buttonText="Stereo Coupling" connectedEdges="11" needsCallback="1"
                radioGroupId="0" state="1"/>
  <LABEL name="new label" id="2b4e89a9f299c472" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="0 150 100 20" edTextCol="ff000000"
         edBkgCol="0" labelText="Left Channel" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Arial"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="3f08f432e7c102bb" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="100 150 100 20" edTextCol="ff000000"
         edBkgCol="0" labelText="Right Channel" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Arial"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <JUCERCOMP name="meterChild" id="bafad9d58caaf2d5" memberName="meterChild"
             virtualName="" explicitFocusOrder="0" pos="200 0 300 200" sourceFile="../../components/MeterComponent.cpp"
             constructorParams=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

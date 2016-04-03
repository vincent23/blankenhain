/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 4.1.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#ifdef PLUGIN_PAN
//[/Headers]

#include "PanAudioProcessorEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
PanAudioProcessorEditor::PanAudioProcessorEditor (PanAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (meterChild = new MeterComponent (p));
    addAndMakeVisible (panningSlider = new Slider ("panningSlider"));
    panningSlider->setRange (-50, 50, 1);
    panningSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    panningSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    panningSlider->setColour (Slider::thumbColourId, Colours::black);
    panningSlider->setColour (Slider::rotarySliderFillColourId, Colour (0x7f000000));
    panningSlider->setColour (Slider::textBoxHighlightColourId, Colour (0x40000000));
    panningSlider->addListener (this);

    addAndMakeVisible (label = new Label ("new label",
                                          TRANS("panning")));
    label->setFont (Font (26.30f, Font::plain));
    label->setJustificationType (Justification::centred);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (monoButton = new ToggleButton ("monoButton"));
    monoButton->setButtonText (TRANS("mono"));
    monoButton->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (420, 300);


    //[Constructor] You can add your own custom stuff here..
	startTimer(100);
    //[/Constructor]
}

PanAudioProcessorEditor::~PanAudioProcessorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    meterChild = nullptr;
    panningSlider = nullptr;
    label = nullptr;
    monoButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void PanAudioProcessorEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void PanAudioProcessorEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    meterChild->setBounds (200, 0, 232, 304);
    panningSlider->setBounds (proportionOfWidth (0.0381f), 24, 168, proportionOfHeight (0.6400f));
    label->setBounds (proportionOfWidth (-0.0381f), proportionOfHeight (0.7200f), proportionOfWidth (0.5405f), proportionOfHeight (0.1500f));
    monoButton->setBounds (72, 256, 64, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void PanAudioProcessorEditor::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == panningSlider)
    {
        //[UserSliderCode_panningSlider] -- add your slider handling code here..
		processor.setPanning(static_cast<float>(sliderThatWasMoved->getValue()));
        //[/UserSliderCode_panningSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void PanAudioProcessorEditor::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == monoButton)
    {
        //[UserButtonCode_monoButton] -- add your button handler code here..
      processor.setMono(!processor.getMono());
        //[/UserButtonCode_monoButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void PanAudioProcessorEditor::timerCallback()
{
	panningSlider->setValue(processor.getPanning(), juce::dontSendNotification);
  //monoButton->setToggleState(processor.getMono());

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

<JUCER_COMPONENT documentType="Component" className="PanAudioProcessorEditor"
                 componentName="" parentClasses="public AudioProcessorEditor, public Timer"
                 constructorParams="PanAudioProcessor&amp; p" variableInitialisers="AudioProcessorEditor(&amp;p), processor(p)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="420" initialHeight="300">
  <BACKGROUND backgroundColour="ffffffff"/>
  <JUCERCOMP name="" id="9bae3019e598c401" memberName="meterChild" virtualName=""
             explicitFocusOrder="0" pos="200 0 232 304" sourceFile="../../components/MeterComponent.cpp"
             constructorParams="p"/>
  <SLIDER name="panningSlider" id="e1fc1f10f0eb0d22" memberName="panningSlider"
          virtualName="" explicitFocusOrder="0" pos="3.81% 24 168 64%"
          thumbcol="ff000000" rotarysliderfill="7f000000" textboxhighlight="40000000"
          min="-50" max="50" int="1" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="2889253ab4f709e" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="-3.81% 72% 54.048% 15%" edTextCol="ff000000"
         edBkgCol="0" labelText="panning" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="26.300000000000000711"
         bold="0" italic="0" justification="36"/>
  <TOGGLEBUTTON name="monoButton" id="31fafbfa9c1ce92d" memberName="monoButton"
                virtualName="" explicitFocusOrder="0" pos="72 256 64 24" buttonText="mono"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
#endif
//[/EndFile]

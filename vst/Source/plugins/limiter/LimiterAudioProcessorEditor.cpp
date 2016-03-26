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
#ifdef PLUGIN_LIMITER
//[/Headers]

#include "LimiterAudioProcessorEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
LimiterAudioProcessorEditor::LimiterAudioProcessorEditor (LimiterAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (meterChild = new MeterComponent());
    addAndMakeVisible (releaseSlider = new Slider ("releaseSlider"));
    releaseSlider->setTooltip (TRANS("in ms"));
    releaseSlider->setRange (0, 1000, 0);
    releaseSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    releaseSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    releaseSlider->addListener (this);

    addAndMakeVisible (releaseText = new Label ("releaseText",
                                                TRANS("Release")));
    releaseText->setFont (Font (15.00f, Font::plain));
    releaseText->setJustificationType (Justification::centred);
    releaseText->setEditable (false, false, false);
    releaseText->setColour (TextEditor::textColourId, Colours::black);
    releaseText->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (thresholdSlider = new Slider ("thresholdSlider"));
    thresholdSlider->setTooltip (TRANS("in dB"));
    thresholdSlider->setRange (-36, 0, 0);
    thresholdSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    thresholdSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    thresholdSlider->addListener (this);

    addAndMakeVisible (thresholdText = new Label ("thresholdText",
                                                  TRANS("Threshold")));
    thresholdText->setFont (Font (15.00f, Font::plain));
    thresholdText->setJustificationType (Justification::centred);
    thresholdText->setEditable (false, false, false);
    thresholdText->setColour (TextEditor::textColourId, Colours::black);
    thresholdText->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (lookaheadSlider = new Slider ("lookaheadSlider"));
    lookaheadSlider->setRange (0, 6, 0);
    lookaheadSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    lookaheadSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    lookaheadSlider->addListener (this);

    addAndMakeVisible (lookaheadText = new Label ("lookaheadText",
                                                  TRANS("Ratio")));
    lookaheadText->setFont (Font (15.00f, Font::plain));
    lookaheadText->setJustificationType (Justification::centred);
    lookaheadText->setEditable (false, false, false);
    lookaheadText->setColour (TextEditor::textColourId, Colours::black);
    lookaheadText->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 300);


    //[Constructor] You can add your own custom stuff here..
    startTimer(100);
    //[/Constructor]
}

LimiterAudioProcessorEditor::~LimiterAudioProcessorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    meterChild = nullptr;
    releaseSlider = nullptr;
    releaseText = nullptr;
    thresholdSlider = nullptr;
    thresholdText = nullptr;
    lookaheadSlider = nullptr;
    lookaheadText = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void LimiterAudioProcessorEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void LimiterAudioProcessorEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    meterChild->setBounds (424, 0, 200, 304);
    releaseSlider->setBounds (120, 16, 96, 88);
    releaseText->setBounds (128, 107, 79, 24);
    thresholdSlider->setBounds (232, 16, 96, 88);
    thresholdText->setBounds (240, 107, 79, 24);
    lookaheadSlider->setBounds (16, 16, 96, 88);
    lookaheadText->setBounds (24, 112, 79, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void LimiterAudioProcessorEditor::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == releaseSlider)
    {
        //[UserSliderCode_releaseSlider] -- add your slider handling code here..
      processor.setRelease(sliderThatWasMoved->getValue());

        //[/UserSliderCode_releaseSlider]
    }
    else if (sliderThatWasMoved == thresholdSlider)
    {
        //[UserSliderCode_thresholdSlider] -- add your slider handling code here..
      processor.setThreshold(sliderThatWasMoved->getValue());
        //[/UserSliderCode_thresholdSlider]
    }
    else if (sliderThatWasMoved == lookaheadSlider)
    {
        //[UserSliderCode_lookaheadSlider] -- add your slider handling code here..
      processor.setLookahead(sliderThatWasMoved->getValue());
        //[/UserSliderCode_lookaheadSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void LimiterAudioProcessorEditor::timerCallback()
{

  lookaheadSlider->setValue(processor.getLookahead(), juce::dontSendNotification);
  releaseSlider->setValue(processor.getRelease(), juce::dontSendNotification);
  thresholdSlider->setValue(processor.getThreshold(), juce::dontSendNotification);


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

<JUCER_COMPONENT documentType="Component" className="LimiterAudioProcessorEditor"
                 componentName="" parentClasses="public AudioProcessorEditor, public Timer"
                 constructorParams="LimiterAudioProcessor&amp; p" variableInitialisers="AudioProcessorEditor(&amp;p), processor(p)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="600" initialHeight="300">
  <BACKGROUND backgroundColour="ffffffff"/>
  <JUCERCOMP name="" id="dc979ea1bc23b53c" memberName="meterChild" virtualName=""
             explicitFocusOrder="0" pos="424 0 200 304" sourceFile="../../components/MeterComponent.cpp"
             constructorParams=""/>
  <SLIDER name="releaseSlider" id="61df89e44cdfef81" memberName="releaseSlider"
          virtualName="" explicitFocusOrder="0" pos="120 16 96 88" tooltip="in ms"
          min="0" max="1000" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="releaseText" id="ccf625701324a5a1" memberName="releaseText"
         virtualName="" explicitFocusOrder="0" pos="128 107 79 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Release" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <SLIDER name="thresholdSlider" id="df8a56a098604715" memberName="thresholdSlider"
          virtualName="" explicitFocusOrder="0" pos="232 16 96 88" tooltip="in dB"
          min="-36" max="0" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="thresholdText" id="d8ec008ec365bd94" memberName="thresholdText"
         virtualName="" explicitFocusOrder="0" pos="240 107 79 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Threshold" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <SLIDER name="lookaheadSlider" id="4939e3fba015c9af" memberName="lookaheadSlider"
          virtualName="" explicitFocusOrder="0" pos="16 16 96 88" min="0"
          max="6" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="lookaheadText" id="8bb6cc62e2932c59" memberName="lookaheadText"
         virtualName="" explicitFocusOrder="0" pos="24 112 79 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Ratio" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
#endif
//[/EndFile]

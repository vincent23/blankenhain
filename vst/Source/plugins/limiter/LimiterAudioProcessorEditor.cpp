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

    addAndMakeVisible (meterChild = new MeterComponent (p));
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

    addAndMakeVisible (lookaheadSlider = new Slider ("lookaheadSlider"));
    lookaheadSlider->setRange (5, 10000, 0);
    lookaheadSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    lookaheadSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    lookaheadSlider->addListener (this);

    addAndMakeVisible (lookaheadText = new Label ("lookaheadText",
                                                  TRANS("Lookahead")));
    lookaheadText->setFont (Font (15.00f, Font::plain));
    lookaheadText->setJustificationType (Justification::centred);
    lookaheadText->setEditable (false, false, false);
    lookaheadText->setColour (TextEditor::textColourId, Colours::black);
    lookaheadText->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (igainSlider = new Slider ("igainSlider"));
    igainSlider->setRange (-5, 15, 0);
    igainSlider->setSliderStyle (Slider::LinearBar);
    igainSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    igainSlider->addListener (this);

    addAndMakeVisible (ogainSlider = new Slider ("ogainSlider"));
    ogainSlider->setRange (-5, 5, 0);
    ogainSlider->setSliderStyle (Slider::LinearBar);
    ogainSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    ogainSlider->addListener (this);

    addAndMakeVisible (igainText = new Label ("igainText",
                                              TRANS("Input Gain")));
    igainText->setFont (Font (15.00f, Font::plain));
    igainText->setJustificationType (Justification::centred);
    igainText->setEditable (false, false, false);
    igainText->setColour (TextEditor::textColourId, Colours::black);
    igainText->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (ogainText = new Label ("ogainText",
                                              TRANS("Output Gain")));
    ogainText->setFont (Font (15.00f, Font::plain));
    ogainText->setJustificationType (Justification::centred);
    ogainText->setEditable (false, false, false);
    ogainText->setColour (TextEditor::textColourId, Colours::black);
    ogainText->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (bypassButton = new ToggleButton ("bypassButton"));
    bypassButton->setButtonText (TRANS("Bypass"));
    bypassButton->addListener (this);

    addAndMakeVisible (suggestedOgainLabel = new TextButton ("suggestedOgainLabel"));
    suggestedOgainLabel->setButtonText (TRANS("suggested: "));
    suggestedOgainLabel->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (650, 300);


    //[Constructor] You can add your own custom stuff here..
    startTimer(80);
    //[/Constructor]
}

LimiterAudioProcessorEditor::~LimiterAudioProcessorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    meterChild = nullptr;
    releaseSlider = nullptr;
    releaseText = nullptr;
    lookaheadSlider = nullptr;
    lookaheadText = nullptr;
    igainSlider = nullptr;
    ogainSlider = nullptr;
    igainText = nullptr;
    ogainText = nullptr;
    bypassButton = nullptr;
    suggestedOgainLabel = nullptr;


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

    meterChild->setBounds (424, 0, 232, 304);
    releaseSlider->setBounds (120, 16, 96, 88);
    releaseText->setBounds (128, 107, 79, 24);
    lookaheadSlider->setBounds (16, 16, 96, 88);
    lookaheadText->setBounds (24, 112, 79, 24);
    igainSlider->setBounds (32, 168, 360, 24);
    ogainSlider->setBounds (32, 224, 360, 24);
    igainText->setBounds (176, 192, 79, 24);
    ogainText->setBounds (176, 256, 79, 24);
    bypassButton->setBounds (232, 16, 160, 24);
    suggestedOgainLabel->setBounds (224, 48, 168, 73);
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
    else if (sliderThatWasMoved == lookaheadSlider)
    {
        //[UserSliderCode_lookaheadSlider] -- add your slider handling code here..
      processor.setLookahead(sliderThatWasMoved->getValue());
        //[/UserSliderCode_lookaheadSlider]
    }
    else if (sliderThatWasMoved == igainSlider)
    {
        //[UserSliderCode_igainSlider] -- add your slider handling code here..
      processor.setIgain(sliderThatWasMoved->getValue());
        //[/UserSliderCode_igainSlider]
    }
    else if (sliderThatWasMoved == ogainSlider)
    {
        //[UserSliderCode_ogainSlider] -- add your slider handling code here..
      processor.setOgain(sliderThatWasMoved->getValue());
        //[/UserSliderCode_ogainSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void LimiterAudioProcessorEditor::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == bypassButton)
    {
        //[UserButtonCode_bypassButton] -- add your button handler code here..
      processor.switchBypass();
        //[/UserButtonCode_bypassButton]
    }
    else if (buttonThatWasClicked == suggestedOgainLabel)
    {
        //[UserButtonCode_suggestedOgainLabel] -- add your button handler code here..
      processor.suggestedOgain = 99999999999;
        //[/UserButtonCode_suggestedOgainLabel]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void LimiterAudioProcessorEditor::timerCallback()
{

  lookaheadSlider->setValue(processor.getLookahead(), juce::dontSendNotification);
  releaseSlider->setValue(processor.getRelease(), juce::dontSendNotification);
  ogainSlider->setValue(processor.getOgain(), juce::dontSendNotification);
  igainSlider->setValue(processor.getIgain(), juce::dontSendNotification);
  suggestedOgainLabel->setButtonText("suggested output gain: " + juce::String(processor.suggestedOgain));

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
                 fixedSize="1" initialWidth="650" initialHeight="300">
  <BACKGROUND backgroundColour="ffffffff"/>
  <JUCERCOMP name="" id="dc979ea1bc23b53c" memberName="meterChild" virtualName=""
             explicitFocusOrder="0" pos="424 0 232 304" sourceFile="../../components/MeterComponent.cpp"
             constructorParams="p"/>
  <SLIDER name="releaseSlider" id="61df89e44cdfef81" memberName="releaseSlider"
          virtualName="" explicitFocusOrder="0" pos="120 16 96 88" tooltip="in ms"
          min="0" max="1000" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="releaseText" id="ccf625701324a5a1" memberName="releaseText"
         virtualName="" explicitFocusOrder="0" pos="128 107 79 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Release" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <SLIDER name="lookaheadSlider" id="4939e3fba015c9af" memberName="lookaheadSlider"
          virtualName="" explicitFocusOrder="0" pos="16 16 96 88" min="5"
          max="10000" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="lookaheadText" id="8bb6cc62e2932c59" memberName="lookaheadText"
         virtualName="" explicitFocusOrder="0" pos="24 112 79 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Lookahead" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <SLIDER name="igainSlider" id="2c3a47a560f54711" memberName="igainSlider"
          virtualName="" explicitFocusOrder="0" pos="32 168 360 24" min="-5"
          max="15" int="0" style="LinearBar" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="ogainSlider" id="10661bb15ced9774" memberName="ogainSlider"
          virtualName="" explicitFocusOrder="0" pos="32 224 360 24" min="-5"
          max="5" int="0" style="LinearBar" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="igainText" id="dcaaa3d61af125ba" memberName="igainText"
         virtualName="" explicitFocusOrder="0" pos="176 192 79 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Input Gain" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <LABEL name="ogainText" id="22d65e1a1134eca9" memberName="ogainText"
         virtualName="" explicitFocusOrder="0" pos="176 256 79 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Output Gain" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <TOGGLEBUTTON name="bypassButton" id="fed4abcd299cb57d" memberName="bypassButton"
                virtualName="" explicitFocusOrder="0" pos="232 16 160 24" buttonText="Bypass"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTBUTTON name="suggestedOgainLabel" id="b2e9b4947a0accb6" memberName="suggestedOgainLabel"
              virtualName="" explicitFocusOrder="0" pos="224 48 168 73" buttonText="suggested: "
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
#endif
//[/EndFile]

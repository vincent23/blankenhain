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
#ifdef PLUGIN_COMPRESSOR
//[/Headers]

#include "CompressorAudioProcessorEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
CompressorAudioProcessorEditor::CompressorAudioProcessorEditor (CompressorAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (meterChild = new MeterComponent());
    addAndMakeVisible (attackSlider = new Slider ("attackSlider"));
    attackSlider->setTooltip (TRANS("in ms"));
    attackSlider->setRange (0, 1000, 0);
    attackSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    attackSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    attackSlider->addListener (this);

    addAndMakeVisible (attackText = new Label ("attackText",
                                               TRANS("Attack")));
    attackText->setFont (Font (15.00f, Font::plain));
    attackText->setJustificationType (Justification::centred);
    attackText->setEditable (false, false, false);
    attackText->setColour (TextEditor::textColourId, Colours::black);
    attackText->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

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

    addAndMakeVisible (ratioSlider = new Slider ("ratioSlider"));
    ratioSlider->setRange (0, 6, 0);
    ratioSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    ratioSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    ratioSlider->addListener (this);

    addAndMakeVisible (ratioText = new Label ("ratioText",
                                              TRANS("Ratio")));
    ratioText->setFont (Font (15.00f, Font::plain));
    ratioText->setJustificationType (Justification::centred);
    ratioText->setEditable (false, false, false);
    ratioText->setColour (TextEditor::textColourId, Colours::black);
    ratioText->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (postgainSlider = new Slider ("ratioSlider"));
    postgainSlider->setRange (0, 24, 0);
    postgainSlider->setSliderStyle (Slider::LinearVertical);
    postgainSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    postgainSlider->addListener (this);

    addAndMakeVisible (postgainText = new Label ("postgainText",
                                                 TRANS("Post\n"
                                                 "Gain")));
    postgainText->setFont (Font (15.00f, Font::plain));
    postgainText->setJustificationType (Justification::centred);
    postgainText->setEditable (false, false, false);
    postgainText->setColour (TextEditor::textColourId, Colours::black);
    postgainText->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 300);


    //[Constructor] You can add your own custom stuff here..
    startTimer(100);
    //[/Constructor]
}

CompressorAudioProcessorEditor::~CompressorAudioProcessorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    meterChild = nullptr;
    attackSlider = nullptr;
    attackText = nullptr;
    releaseSlider = nullptr;
    releaseText = nullptr;
    thresholdSlider = nullptr;
    thresholdText = nullptr;
    ratioSlider = nullptr;
    ratioText = nullptr;
    postgainSlider = nullptr;
    postgainText = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void CompressorAudioProcessorEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void CompressorAudioProcessorEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    meterChild->setBounds (424, 0, 200, 304);
    attackSlider->setBounds (8, 16, 96, 88);
    attackText->setBounds (16, 107, 79, 24);
    releaseSlider->setBounds (120, 16, 96, 88);
    releaseText->setBounds (128, 107, 79, 24);
    thresholdSlider->setBounds (232, 16, 96, 88);
    thresholdText->setBounds (240, 107, 79, 24);
    ratioSlider->setBounds (8, 152, 96, 88);
    ratioText->setBounds (16, 248, 79, 24);
    postgainSlider->setBounds (360, 8, 40, 232);
    postgainText->setBounds (336, 256, 87, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void CompressorAudioProcessorEditor::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == attackSlider)
    {
        //[UserSliderCode_attackSlider] -- add your slider handling code here..
      processor.setAttack(sliderThatWasMoved->getValue());
        //[/UserSliderCode_attackSlider]
    }
    else if (sliderThatWasMoved == releaseSlider)
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
    else if (sliderThatWasMoved == ratioSlider)
    {
        //[UserSliderCode_ratioSlider] -- add your slider handling code here..
      processor.setRatio(sliderThatWasMoved->getValue());

        //[/UserSliderCode_ratioSlider]
    }
    else if (sliderThatWasMoved == postgainSlider)
    {
        //[UserSliderCode_postgainSlider] -- add your slider handling code here..
      processor.setPostgain(sliderThatWasMoved->getValue());
        //[/UserSliderCode_postgainSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void CompressorAudioProcessorEditor::timerCallback()
{
  attackSlider->setValue(processor.getAttack(), juce::dontSendNotification);
  ratioSlider->setValue(processor.getRatio(), juce::dontSendNotification);
  releaseSlider->setValue(processor.getRelease(), juce::dontSendNotification);
  thresholdSlider->setValue(processor.getThreshold(), juce::dontSendNotification);
  postgainSlider->setValue(processor.getPostgain(), juce::dontSendNotification);

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

<JUCER_COMPONENT documentType="Component" className="CompressorAudioProcessorEditor"
                 componentName="" parentClasses="public AudioProcessorEditor, public Timer"
                 constructorParams="CompressorAudioProcessor&amp; p" variableInitialisers="AudioProcessorEditor(&amp;p), processor(p)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="600" initialHeight="300">
  <BACKGROUND backgroundColour="ffffffff"/>
  <JUCERCOMP name="" id="dc979ea1bc23b53c" memberName="meterChild" virtualName=""
             explicitFocusOrder="0" pos="424 0 200 304" sourceFile="../../components/MeterComponent.cpp"
             constructorParams=""/>
  <SLIDER name="attackSlider" id="bc4747722abe5c6" memberName="attackSlider"
          virtualName="" explicitFocusOrder="0" pos="8 16 96 88" tooltip="in ms"
          min="0" max="1000" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="attackText" id="9e80b85aebab9868" memberName="attackText"
         virtualName="" explicitFocusOrder="0" pos="16 107 79 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Attack" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
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
  <SLIDER name="ratioSlider" id="4939e3fba015c9af" memberName="ratioSlider"
          virtualName="" explicitFocusOrder="0" pos="8 152 96 88" min="0"
          max="6" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="ratioText" id="8bb6cc62e2932c59" memberName="ratioText"
         virtualName="" explicitFocusOrder="0" pos="16 248 79 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Ratio" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <SLIDER name="ratioSlider" id="606f219d732ad1ca" memberName="postgainSlider"
          virtualName="" explicitFocusOrder="0" pos="360 8 40 232" min="0"
          max="24" int="0" style="LinearVertical" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="postgainText" id="5540ebd35c93856b" memberName="postgainText"
         virtualName="" explicitFocusOrder="0" pos="336 256 87 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Post&#10;Gain" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
#endif
//[/EndFile]

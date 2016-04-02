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
#ifdef PLUGIN_DELAY
//[/Headers]

#include "DelayAudioProcessorEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
DelayAudioProcessorEditor::DelayAudioProcessorEditor (DelayAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (freeLengthSlider = new Slider ("freeLengthSlider"));
    freeLengthSlider->setExplicitFocusOrder (1000);
    freeLengthSlider->setRange (5, 5000, 0);
    freeLengthSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    freeLengthSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    freeLengthSlider->addListener (this);

    addAndMakeVisible (feedbackSlider = new Slider ("feedbackSlider"));
    feedbackSlider->setRange (0, 1, 0);
    feedbackSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    feedbackSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    feedbackSlider->addListener (this);

    addAndMakeVisible (panSlider = new Slider ("panSlider"));
    panSlider->setRange (-1, 1, 0);
    panSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    panSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    panSlider->addListener (this);

    addAndMakeVisible (panLabel = new Label ("panLabel",
                                             TRANS("Pan")));
    panLabel->setFont (Font (15.00f, Font::plain));
    panLabel->setJustificationType (Justification::centredLeft);
    panLabel->setEditable (false, false, false);
    panLabel->setColour (TextEditor::textColourId, Colours::black);
    panLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (feedbackLabel = new Label ("feedbackLabel",
                                                  TRANS("Feedback")));
    feedbackLabel->setFont (Font (15.00f, Font::plain));
    feedbackLabel->setJustificationType (Justification::centred);
    feedbackLabel->setEditable (false, false, false);
    feedbackLabel->setColour (TextEditor::textColourId, Colours::black);
    feedbackLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (freeLengthLabel = new Label ("freeLengthLabel",
                                                    TRANS("Free")));
    freeLengthLabel->setFont (Font (15.00f, Font::plain));
    freeLengthLabel->setJustificationType (Justification::centred);
    freeLengthLabel->setEditable (false, false, false);
    freeLengthLabel->setColour (TextEditor::textColourId, Colours::black);
    freeLengthLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (meterChild = new MeterComponent (p));
    addAndMakeVisible (hostBPMLabel = new Label ("hostBPMLabel",
                                                 TRANS("Host BPM:")));
    hostBPMLabel->setFont (Font (15.00f, Font::plain));
    hostBPMLabel->setJustificationType (Justification::centredLeft);
    hostBPMLabel->setEditable (false, false, false);
    hostBPMLabel->setColour (TextEditor::textColourId, Colours::black);
    hostBPMLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (valueLabel = new Label ("valueLabel",
                                               TRANS("4:\n"
                                               "2:\n"
                                               "1:\n"
                                               "1/2:\n"
                                               "1/4:\n"
                                               "1/8:\n"
                                               "1/16:\n"
                                               "1/32:")));
    valueLabel->setFont (Font (15.00f, Font::plain));
    valueLabel->setJustificationType (Justification::centredLeft);
    valueLabel->setEditable (false, false, false);
    valueLabel->setColour (TextEditor::textColourId, Colours::black);
    valueLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (drywetSlider = new Slider ("drywetSlider"));
    drywetSlider->setRange (0, 1, 0);
    drywetSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    drywetSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    drywetSlider->addListener (this);

    addAndMakeVisible (label = new Label ("new label",
                                          TRANS("Dry/Wet")));
    label->setFont (Font (15.00f, Font::plain));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (650, 300);


    //[Constructor] You can add your own custom stuff here..
    startTimer(100);
    //[/Constructor]
}

DelayAudioProcessorEditor::~DelayAudioProcessorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    freeLengthSlider = nullptr;
    feedbackSlider = nullptr;
    panSlider = nullptr;
    panLabel = nullptr;
    feedbackLabel = nullptr;
    freeLengthLabel = nullptr;
    meterChild = nullptr;
    hostBPMLabel = nullptr;
    valueLabel = nullptr;
    drywetSlider = nullptr;
    label = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void DelayAudioProcessorEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void DelayAudioProcessorEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    freeLengthSlider->setBounds (16, 16, 87, 88);
    feedbackSlider->setBounds (320, 16, 80, 88);
    panSlider->setBounds (208, 16, 87, 88);
    panLabel->setBounds (232, 112, 40, 24);
    feedbackLabel->setBounds (320, 112, 80, 24);
    freeLengthLabel->setBounds (32, 112, 56, 24);
    meterChild->setBounds (424, 0, 228, 304);
    hostBPMLabel->setBounds (24, 144, 150, 24);
    valueLabel->setBounds (24, 168, 144, 128);
    drywetSlider->setBounds (320, 152, 88, 96);
    label->setBounds (328, 256, 79, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void DelayAudioProcessorEditor::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == freeLengthSlider)
    {
        //[UserSliderCode_freeLengthSlider] -- add your slider handling code here..
      processor.setLength(sliderThatWasMoved->getValue());
        //[/UserSliderCode_freeLengthSlider]
    }
    else if (sliderThatWasMoved == feedbackSlider)
    {
        //[UserSliderCode_feedbackSlider] -- add your slider handling code here..
      processor.setFeedback(sliderThatWasMoved->getValue());
        //[/UserSliderCode_feedbackSlider]
    }
    else if (sliderThatWasMoved == panSlider)
    {
        //[UserSliderCode_panSlider] -- add your slider handling code here..
      processor.setPan(sliderThatWasMoved->getValue());
        //[/UserSliderCode_panSlider]
    }
    else if (sliderThatWasMoved == drywetSlider)
    {
        //[UserSliderCode_drywetSlider] -- add your slider handling code here..
      processor.setDrywet(sliderThatWasMoved->getValue());
        //[/UserSliderCode_drywetSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void DelayAudioProcessorEditor::timerCallback()
{
  freeLengthSlider->setValue(processor.getLength(), juce::dontSendNotification);
  panSlider->setValue(processor.getPan(), juce::dontSendNotification);
  feedbackSlider->setValue(processor.getFeedback(), juce::dontSendNotification);
  drywetSlider->setValue(processor.getDrywet(), juce::dontSendNotification);

  float bpm = processor.getBPM();
  float value = 44100.f / (bpm / 60.f);
  valueLabel->setText(
      juce::String("4: "       + juce::String(value / 16.f))
    + juce::String("\n2: "     + juce::String(value / 8.f))
    + juce::String("\n1: "     + juce::String(value / 4.f))
    + juce::String("\n1\/2:"   + juce::String(value / 2.f))
    + juce::String("\n1\/4: "  + juce::String(value))
    + juce::String("\n1\/8: "  + juce::String(value *2.f))
    + juce::String("\n1\/16: " + juce::String(value*4.f))
    + juce::String("\n1\/32: " + juce::String(value*8.f))
    , juce::dontSendNotification);

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

<JUCER_COMPONENT documentType="Component" className="DelayAudioProcessorEditor"
                 componentName="" parentClasses="public AudioProcessorEditor, public Timer"
                 constructorParams="DelayAudioProcessor&amp; p" variableInitialisers="AudioProcessorEditor(&amp;p), processor(p)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="650" initialHeight="300">
  <BACKGROUND backgroundColour="ffffffff"/>
  <SLIDER name="freeLengthSlider" id="a7d18825f760554a" memberName="freeLengthSlider"
          virtualName="" explicitFocusOrder="1000" pos="16 16 87 88" min="5"
          max="5000" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="feedbackSlider" id="e376e1777a473939" memberName="feedbackSlider"
          virtualName="" explicitFocusOrder="0" pos="320 16 80 88" min="0"
          max="1" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="panSlider" id="c1e9498d831e5114" memberName="panSlider"
          virtualName="" explicitFocusOrder="0" pos="208 16 87 88" min="-1"
          max="1" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="panLabel" id="f2bfdab03c02a625" memberName="panLabel" virtualName=""
         explicitFocusOrder="0" pos="232 112 40 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Pan" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="feedbackLabel" id="b16d3982198964a0" memberName="feedbackLabel"
         virtualName="" explicitFocusOrder="0" pos="320 112 80 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Feedback" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <LABEL name="freeLengthLabel" id="23c7aeccbd52bbad" memberName="freeLengthLabel"
         virtualName="" explicitFocusOrder="0" pos="32 112 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Free" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <JUCERCOMP name="" id="7c85bdf0fe95a90f" memberName="meterChild" virtualName=""
             explicitFocusOrder="0" pos="424 0 228 304" sourceFile="../../components/MeterComponent.cpp"
             constructorParams="p"/>
  <LABEL name="hostBPMLabel" id="8cfdbbfeb6e21855" memberName="hostBPMLabel"
         virtualName="" explicitFocusOrder="0" pos="24 144 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Host BPM:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="valueLabel" id="874cf92743531556" memberName="valueLabel"
         virtualName="" explicitFocusOrder="0" pos="24 168 144 128" edTextCol="ff000000"
         edBkgCol="0" labelText="4:&#10;2:&#10;1:&#10;1/2:&#10;1/4:&#10;1/8:&#10;1/16:&#10;1/32:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <SLIDER name="drywetSlider" id="5f2593ed00a55a1e" memberName="drywetSlider"
          virtualName="" explicitFocusOrder="0" pos="320 152 88 96" min="0"
          max="1" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="7c832236bf4bcc95" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="328 256 79 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Dry/Wet" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
#endif
//[/EndFile]

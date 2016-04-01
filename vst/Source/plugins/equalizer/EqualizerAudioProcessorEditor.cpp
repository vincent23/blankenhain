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
#ifdef PLUGIN_EQUALIZER
//[/Headers]

#include "EqualizerAudioProcessorEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
EqualizerAudioProcessorEditor::EqualizerAudioProcessorEditor (EqualizerAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (lowSlider = new Slider ("lowSlider"));
    lowSlider->setExplicitFocusOrder (1000);
    lowSlider->setRange (-12, 12, 0);
    lowSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    lowSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    lowSlider->addListener (this);

    addAndMakeVisible (midSlider = new Slider ("midSlider"));
    midSlider->setRange (-12, 12, 0);
    midSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    midSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    midSlider->addListener (this);

    addAndMakeVisible (highFreqSlider = new Slider ("highFreqSlider"));
    highFreqSlider->setRange (40, 20000, 0);
    highFreqSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    highFreqSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    highFreqSlider->addListener (this);

    addAndMakeVisible (lowFreqSlider = new Slider ("lowFreqSlider"));
    lowFreqSlider->setRange (40, 20000, 0);
    lowFreqSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    lowFreqSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    lowFreqSlider->addListener (this);

    addAndMakeVisible (highSlider = new Slider ("highSlider"));
    highSlider->setRange (-12, 12, 0);
    highSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    highSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    highSlider->addListener (this);

    addAndMakeVisible (highLabel = new Label ("highLabel",
                                              TRANS("High")));
    highLabel->setFont (Font (15.00f, Font::plain));
    highLabel->setJustificationType (Justification::centredLeft);
    highLabel->setEditable (false, false, false);
    highLabel->setColour (TextEditor::textColourId, Colours::black);
    highLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (highFreqLabel = new Label ("highFreqLabel",
                                                  TRANS("High Freq")));
    highFreqLabel->setFont (Font (15.00f, Font::plain));
    highFreqLabel->setJustificationType (Justification::centred);
    highFreqLabel->setEditable (false, false, false);
    highFreqLabel->setColour (TextEditor::textColourId, Colours::black);
    highFreqLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (midLabel = new Label ("midLabel",
                                             TRANS("Mid")));
    midLabel->setFont (Font (15.00f, Font::plain));
    midLabel->setJustificationType (Justification::centredLeft);
    midLabel->setEditable (false, false, false);
    midLabel->setColour (TextEditor::textColourId, Colours::black);
    midLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (lowFreqLabel = new Label ("lowFreqLabel",
                                                 TRANS("Low Freq")));
    lowFreqLabel->setFont (Font (15.00f, Font::plain));
    lowFreqLabel->setJustificationType (Justification::centred);
    lowFreqLabel->setEditable (false, false, false);
    lowFreqLabel->setColour (TextEditor::textColourId, Colours::black);
    lowFreqLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (lowLabel = new Label ("lowLabel",
                                             TRANS("Low")));
    lowLabel->setFont (Font (15.00f, Font::plain));
    lowLabel->setJustificationType (Justification::centred);
    lowLabel->setEditable (false, false, false);
    lowLabel->setColour (TextEditor::textColourId, Colours::black);
    lowLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (meterChild = new MeterComponent (p));

    //[UserPreSize]
    //[/UserPreSize]

    setSize (650, 300);


    //[Constructor] You can add your own custom stuff here..
    startTimer(100);
    //[/Constructor]
}

EqualizerAudioProcessorEditor::~EqualizerAudioProcessorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    lowSlider = nullptr;
    midSlider = nullptr;
    highFreqSlider = nullptr;
    lowFreqSlider = nullptr;
    highSlider = nullptr;
    highLabel = nullptr;
    highFreqLabel = nullptr;
    midLabel = nullptr;
    lowFreqLabel = nullptr;
    lowLabel = nullptr;
    meterChild = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void EqualizerAudioProcessorEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void EqualizerAudioProcessorEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    lowSlider->setBounds (16, 16, 87, 88);
    midSlider->setBounds (112, 16, 87, 88);
    highFreqSlider->setBounds (208, 160, 80, 88);
    lowFreqSlider->setBounds (16, 160, 87, 88);
    highSlider->setBounds (208, 16, 87, 88);
    highLabel->setBounds (232, 120, 40, 24);
    highFreqLabel->setBounds (208, 256, 80, 24);
    midLabel->setBounds (136, 120, 40, 24);
    lowFreqLabel->setBounds (24, 256, 72, 24);
    lowLabel->setBounds (32, 120, 56, 24);
    meterChild->setBounds (424, 0, 228, 304);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void EqualizerAudioProcessorEditor::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == lowSlider)
    {
        //[UserSliderCode_lowSlider] -- add your slider handling code here..
      processor.setLow(sliderThatWasMoved->getValue());
        //[/UserSliderCode_lowSlider]
    }
    else if (sliderThatWasMoved == midSlider)
    {
        //[UserSliderCode_midSlider] -- add your slider handling code here..
      processor.setMid(sliderThatWasMoved->getValue());
        //[/UserSliderCode_midSlider]
    }
    else if (sliderThatWasMoved == highFreqSlider)
    {
        //[UserSliderCode_highFreqSlider] -- add your slider handling code here..
      processor.setHighFreq(sliderThatWasMoved->getValue());
        //[/UserSliderCode_highFreqSlider]
    }
    else if (sliderThatWasMoved == lowFreqSlider)
    {
        //[UserSliderCode_lowFreqSlider] -- add your slider handling code here..
      processor.setLowFreq(sliderThatWasMoved->getValue());
        //[/UserSliderCode_lowFreqSlider]
    }
    else if (sliderThatWasMoved == highSlider)
    {
        //[UserSliderCode_highSlider] -- add your slider handling code here..
      processor.setHigh(sliderThatWasMoved->getValue());
        //[/UserSliderCode_highSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void EqualizerAudioProcessorEditor::timerCallback()
{
  lowSlider->setValue(processor.getLow(), juce::dontSendNotification);
  midSlider->setValue(processor.getMid(), juce::dontSendNotification);
  highSlider->setValue(processor.getHigh(), juce::dontSendNotification);
  lowFreqSlider->setValue(processor.getLowFreq(), juce::dontSendNotification);
  highFreqSlider->setValue(processor.getHighFreq(), juce::dontSendNotification);

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

<JUCER_COMPONENT documentType="Component" className="EqualizerAudioProcessorEditor"
                 componentName="" parentClasses="public AudioProcessorEditor, public Timer"
                 constructorParams="EqualizerAudioProcessor&amp; p" variableInitialisers="AudioProcessorEditor(&amp;p), processor(p)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="650" initialHeight="300">
  <BACKGROUND backgroundColour="ffffffff"/>
  <SLIDER name="lowSlider" id="a7d18825f760554a" memberName="lowSlider"
          virtualName="" explicitFocusOrder="1000" pos="16 16 87 88" min="-12"
          max="12" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="midSlider" id="24808a697e6da4a1" memberName="midSlider"
          virtualName="" explicitFocusOrder="0" pos="112 16 87 88" min="-12"
          max="12" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="highFreqSlider" id="e376e1777a473939" memberName="highFreqSlider"
          virtualName="" explicitFocusOrder="0" pos="208 160 80 88" min="40"
          max="20000" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="lowFreqSlider" id="3250ce1c85e757db" memberName="lowFreqSlider"
          virtualName="" explicitFocusOrder="0" pos="16 160 87 88" min="40"
          max="20000" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="highSlider" id="c1e9498d831e5114" memberName="highSlider"
          virtualName="" explicitFocusOrder="0" pos="208 16 87 88" min="-12"
          max="12" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="highLabel" id="f2bfdab03c02a625" memberName="highLabel"
         virtualName="" explicitFocusOrder="0" pos="232 120 40 24" edTextCol="ff000000"
         edBkgCol="0" labelText="High" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="highFreqLabel" id="b16d3982198964a0" memberName="highFreqLabel"
         virtualName="" explicitFocusOrder="0" pos="208 256 80 24" edTextCol="ff000000"
         edBkgCol="0" labelText="High Freq" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <LABEL name="midLabel" id="b20e0faa32576009" memberName="midLabel" virtualName=""
         explicitFocusOrder="0" pos="136 120 40 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Mid" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="lowFreqLabel" id="ac3bf0f7db1792f5" memberName="lowFreqLabel"
         virtualName="" explicitFocusOrder="0" pos="24 256 72 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Low Freq" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <LABEL name="lowLabel" id="23c7aeccbd52bbad" memberName="lowLabel" virtualName=""
         explicitFocusOrder="0" pos="32 120 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Low" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <JUCERCOMP name="" id="7c85bdf0fe95a90f" memberName="meterChild" virtualName=""
             explicitFocusOrder="0" pos="424 0 228 304" sourceFile="../../components/MeterComponent.cpp"
             constructorParams="p"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
#endif
//[/EndFile]

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
#ifdef PLUGIN_NONLIN_DISTORTION
//[/Headers]

#include "NonlinDistortionAudioProcessorEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
NonlinDistortionAudioProcessorEditor::NonlinDistortionAudioProcessorEditor (NonlinDistortionAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (meterChild = new MeterComponent (p));
    addAndMakeVisible (distortionBox = new ComboBox ("Distortion"));
    distortionBox->setEditableText (false);
    distortionBox->setJustificationType (Justification::centredLeft);
    distortionBox->setTextWhenNothingSelected (String());
    distortionBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    distortionBox->addItem (TRANS("none"), 1);
    distortionBox->addItem (TRANS("Araya & Suyama"), 2);
    distortionBox->addItem (TRANS("Doidic et al. symmetric"), 3);
    distortionBox->addItem (TRANS("Doidic et al. asymmetric"), 4);
    distortionBox->addListener (this);

    addAndMakeVisible (intensitySlider = new Slider ("intensitySlider"));
    intensitySlider->setRange (1, 10, 1);
    intensitySlider->setSliderStyle (Slider::IncDecButtons);
    intensitySlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    intensitySlider->addListener (this);

    addAndMakeVisible (dsitortionLabel = new Label ("new label",
                                                    TRANS("Nonlinearity Distortion")));
    dsitortionLabel->setFont (Font (15.00f, Font::plain));
    dsitortionLabel->setJustificationType (Justification::centred);
    dsitortionLabel->setEditable (false, false, false);
    dsitortionLabel->setColour (TextEditor::textColourId, Colours::black);
    dsitortionLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (intensityLabel = new Label ("intensityLabel",
                                                   TRANS("Intensity (number of transformations)")));
    intensityLabel->setFont (Font (15.00f, Font::plain));
    intensityLabel->setJustificationType (Justification::centred);
    intensityLabel->setEditable (false, false, false);
    intensityLabel->setColour (TextEditor::textColourId, Colours::black);
    intensityLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (inputGainLabel = new Label ("inputGainLabel",
                                                   TRANS("Input Gain")));
    inputGainLabel->setFont (Font (15.00f, Font::plain));
    inputGainLabel->setJustificationType (Justification::centred);
    inputGainLabel->setEditable (false, false, false);
    inputGainLabel->setColour (TextEditor::textColourId, Colours::black);
    inputGainLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (inputGainSlider = new Slider ("inputGainSlider"));
    inputGainSlider->setRange (-12, 12, 0);
    inputGainSlider->setSliderStyle (Slider::LinearHorizontal);
    inputGainSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    inputGainSlider->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (650, 300);


    //[Constructor] You can add your own custom stuff here..
    startTimer(100);
    //[/Constructor]
}

NonlinDistortionAudioProcessorEditor::~NonlinDistortionAudioProcessorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    meterChild = nullptr;
    distortionBox = nullptr;
    intensitySlider = nullptr;
    dsitortionLabel = nullptr;
    intensityLabel = nullptr;
    inputGainLabel = nullptr;
    inputGainSlider = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void NonlinDistortionAudioProcessorEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void NonlinDistortionAudioProcessorEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    meterChild->setBounds (424, 0, 228, 304);
    distortionBox->setBounds (88, 40, 264, 24);
    intensitySlider->setBounds (144, 112, 150, 24);
    dsitortionLabel->setBounds (144, 8, 150, 24);
    intensityLabel->setBounds (80, 72, 280, 40);
    inputGainLabel->setBounds (144, 152, 150, 24);
    inputGainSlider->setBounds (80, 184, 280, 48);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void NonlinDistortionAudioProcessorEditor::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == distortionBox)
    {
        //[UserComboBoxCode_distortionBox] -- add your combo box handling code here..
      if (comboBoxThatHasChanged->getSelectedId() == 1 || comboBoxThatHasChanged->getSelectedId() == 0)
      {
        processor.setAlg(0u);
      }
      else if (comboBoxThatHasChanged->getSelectedId() == 2)
      {
        processor.setAlg(1u);
      }
      else if (comboBoxThatHasChanged->getSelectedId() == 3)
      {
        processor.setAlg(2u);
      }
      else if (comboBoxThatHasChanged->getSelectedId() == 4)
      {
        processor.setAlg(3u);
      }
        //[/UserComboBoxCode_distortionBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void NonlinDistortionAudioProcessorEditor::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == intensitySlider)
    {
        //[UserSliderCode_intensitySlider] -- add your slider handling code here..
      processor.setIterations(static_cast<size_t>(sliderThatWasMoved->getValue()));
        //[/UserSliderCode_intensitySlider]
    }
    else if (sliderThatWasMoved == inputGainSlider)
    {
        //[UserSliderCode_inputGainSlider] -- add your slider handling code here..
      processor.setIngain(static_cast<float>(sliderThatWasMoved->getValue()));
        //[/UserSliderCode_inputGainSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void NonlinDistortionAudioProcessorEditor::timerCallback()
{
  intensitySlider->setValue(processor.getIterations(), juce::dontSendNotification);
  inputGainSlider->setValue(processor.getIngain(), juce::dontSendNotification);


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

<JUCER_COMPONENT documentType="Component" className="NonlinDistortionAudioProcessorEditor"
                 componentName="" parentClasses="public AudioProcessorEditor, public Timer"
                 constructorParams="NonlinDistortionAudioProcessor&amp; p" variableInitialisers="AudioProcessorEditor(&amp;p), processor(p)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="650" initialHeight="300">
  <BACKGROUND backgroundColour="ffffffff"/>
  <JUCERCOMP name="" id="7c85bdf0fe95a90f" memberName="meterChild" virtualName=""
             explicitFocusOrder="0" pos="424 0 228 304" sourceFile="../../components/MeterComponent.cpp"
             constructorParams="p"/>
  <COMBOBOX name="Distortion" id="127364f694f991ab" memberName="distortionBox"
            virtualName="" explicitFocusOrder="0" pos="88 40 264 24" editable="0"
            layout="33" items="none&#10;Araya &amp; Suyama&#10;Doidic et al. symmetric&#10;Doidic et al. asymmetric"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="intensitySlider" id="934964235bf27b99" memberName="intensitySlider"
          virtualName="" explicitFocusOrder="0" pos="144 112 150 24" min="1"
          max="10" int="1" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="2029d9c134e6c6cf" memberName="dsitortionLabel"
         virtualName="" explicitFocusOrder="0" pos="144 8 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Nonlinearity Distortion" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="intensityLabel" id="5647bf98bc1f91e9" memberName="intensityLabel"
         virtualName="" explicitFocusOrder="0" pos="80 72 280 40" edTextCol="ff000000"
         edBkgCol="0" labelText="Intensity (number of transformations)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="inputGainLabel" id="3bfdf38c2f98a8f8" memberName="inputGainLabel"
         virtualName="" explicitFocusOrder="0" pos="144 152 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Input Gain" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <SLIDER name="inputGainSlider" id="e4ee677840dd6243" memberName="inputGainSlider"
          virtualName="" explicitFocusOrder="0" pos="80 184 280 48" min="-12"
          max="12" int="0" style="LinearHorizontal" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
#endif
//[/EndFile]

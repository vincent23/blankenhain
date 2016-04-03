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
#include "ledMeterComponent.h"
//[/Headers]

#include "MeterComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MeterComponent::MeterComponent (BlankenhainAudioProcessor& p)
    : processor(p)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    setName ("meterChild");
    addAndMakeVisible (curLeft = new Label ("curLeft",
                                            TRANS("? dB")));
    curLeft->setFont (Font (12.00f, Font::plain));
    curLeft->setJustificationType (Justification::centred);
    curLeft->setEditable (false, false, false);
    curLeft->setColour (Label::backgroundColourId, Colour (0xff7e7e7e));
    curLeft->setColour (TextEditor::textColourId, Colours::black);
    curLeft->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (curRight = new Label ("curRight",
                                             TRANS("? dB")));
    curRight->setFont (Font (12.00f, Font::plain));
    curRight->setJustificationType (Justification::centred);
    curRight->setEditable (false, false, false);
    curRight->setColour (Label::backgroundColourId, Colour (0xff7e7e7e));
    curRight->setColour (TextEditor::textColourId, Colours::black);
    curRight->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (peakLeft = new Label ("peakLeft",
                                             TRANS("? dB")));
    peakLeft->setFont (Font (12.00f, Font::plain));
    peakLeft->setJustificationType (Justification::centred);
    peakLeft->setEditable (false, false, false);
    peakLeft->setColour (Label::backgroundColourId, Colour (0xff7e7e7e));
    peakLeft->setColour (TextEditor::textColourId, Colours::black);
    peakLeft->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (peakRight = new Label ("peakRight",
                                              TRANS("? dB")));
    peakRight->setFont (Font (12.00f, Font::plain));
    peakRight->setJustificationType (Justification::centred);
    peakRight->setEditable (false, false, false);
    peakRight->setColour (Label::backgroundColourId, Colour (0xff7e7e7e));
    peakRight->setColour (TextEditor::textColourId, Colours::black);
    peakRight->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (RMSLeft = new Label ("RMSLeft",
                                            TRANS("? dB")));
    RMSLeft->setFont (Font (12.00f, Font::plain));
    RMSLeft->setJustificationType (Justification::centred);
    RMSLeft->setEditable (false, false, false);
    RMSLeft->setColour (Label::backgroundColourId, Colour (0xff7e7e7e));
    RMSLeft->setColour (TextEditor::textColourId, Colours::black);
    RMSLeft->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (RMSRight = new Label ("RMSRight",
                                             TRANS("? dB")));
    RMSRight->setFont (Font (12.00f, Font::plain));
    RMSRight->setJustificationType (Justification::centred);
    RMSRight->setEditable (false, false, false);
    RMSRight->setColour (Label::backgroundColourId, Colour (0xff7e7e7e));
    RMSRight->setColour (TextEditor::textColourId, Colours::black);
    RMSRight->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (curText = new Label ("curText",
                                            TRANS("cur")));
    curText->setFont (Font (15.00f, Font::plain));
    curText->setJustificationType (Justification::centred);
    curText->setEditable (false, false, false);
    curText->setColour (Label::backgroundColourId, Colours::black);
    curText->setColour (Label::textColourId, Colours::white);
    curText->setColour (Label::outlineColourId, Colours::white);
    curText->setColour (TextEditor::textColourId, Colours::black);
    curText->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (peakText = new Label ("peakText",
                                             TRANS("Peak")));
    peakText->setFont (Font (15.00f, Font::plain));
    peakText->setJustificationType (Justification::centred);
    peakText->setEditable (false, false, false);
    peakText->setColour (Label::backgroundColourId, Colours::black);
    peakText->setColour (Label::textColourId, Colours::white);
    peakText->setColour (Label::outlineColourId, Colours::white);
    peakText->setColour (TextEditor::textColourId, Colours::black);
    peakText->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (rmsText = new Label ("rmsText",
                                            TRANS("RMS")));
    rmsText->setFont (Font (15.00f, Font::plain));
    rmsText->setJustificationType (Justification::centred);
    rmsText->setEditable (false, false, false);
    rmsText->setColour (Label::backgroundColourId, Colours::black);
    rmsText->setColour (Label::textColourId, Colours::white);
    rmsText->setColour (Label::outlineColourId, Colours::white);
    rmsText->setColour (TextEditor::textColourId, Colours::black);
    rmsText->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (ledPeakMeterChild = new ledPeakMeterComponent());
    addAndMakeVisible (inSlider = new Slider ("inSlider"));
    inSlider->setRange (-12, 12, 0);
    inSlider->setSliderStyle (Slider::LinearVertical);
    inSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    inSlider->addListener (this);

    addAndMakeVisible (outSlider = new Slider ("outSlider"));
    outSlider->setRange (-12, 12, 0);
    outSlider->setSliderStyle (Slider::LinearVertical);
    outSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    outSlider->addListener (this);

    addAndMakeVisible (Inlabel = new Label ("Inlabel",
                                            TRANS("In")));
    Inlabel->setFont (Font (15.00f, Font::plain));
    Inlabel->setJustificationType (Justification::centredLeft);
    Inlabel->setEditable (false, false, false);
    Inlabel->setColour (Label::textColourId, Colours::white);
    Inlabel->setColour (TextEditor::textColourId, Colours::black);
    Inlabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (OutLabel = new Label ("OutLabel",
                                             TRANS("Out")));
    OutLabel->setFont (Font (15.00f, Font::plain));
    OutLabel->setJustificationType (Justification::centredLeft);
    OutLabel->setEditable (false, false, false);
    OutLabel->setColour (Label::textColourId, Colours::white);
    OutLabel->setColour (TextEditor::textColourId, Colours::black);
    OutLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (toggleButton = new ToggleButton ("new toggle button"));
    toggleButton->setButtonText (TRANS("bypass"));
    toggleButton->addListener (this);
    toggleButton->setColour (ToggleButton::textColourId, Colours::white);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (220, 300);


    //[Constructor] You can add your own custom stuff here..
	peakText->setInterceptsMouseClicks(false, false);
	peakRight->setInterceptsMouseClicks(false, false);
	peakLeft->setInterceptsMouseClicks(false, false);
	bareValues.resize(6, 0.f);
	decayingValues.resize(6, 0.f);
    //[/Constructor]
}

MeterComponent::~MeterComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    curLeft = nullptr;
    curRight = nullptr;
    peakLeft = nullptr;
    peakRight = nullptr;
    RMSLeft = nullptr;
    RMSRight = nullptr;
    curText = nullptr;
    peakText = nullptr;
    rmsText = nullptr;
    ledPeakMeterChild = nullptr;
    inSlider = nullptr;
    outSlider = nullptr;
    Inlabel = nullptr;
    OutLabel = nullptr;
    toggleButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MeterComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::black);

    g.setColour (Colours::black);
    g.fillRect (120, 10, 40, 182);

    g.setColour (Colours::white);
    g.drawRect (120, 10, 40, 182, 5);

    g.setColour (Colours::black);
    g.fillRect (8, 10, 40, 182);

    g.setColour (Colours::white);
    g.drawRect (8, 10, 40, 182, 5);

    //[UserPaint] Add your own custom painting code here..
	g.setGradientFill(ColourGradient(Colours::red,
		0.0f, 0.0f,
		Colours::green,
		0.0f, 100.0f,
		false));

	//g.fillRect(13, 15 , 30, 174);

	float tempValL = ( decayingValues[0] * -174.f + 174.f ) < 0.f ? 0.f : (decayingValues[0] * -174.f + 174.f);
	float tempValR = (decayingValues[1] * -174.f + 174.f ) < 0.f ? 0.f : (decayingValues[1] * -174.f + 174.f);

	g.fillRect(13, int (15 + tempValL), 30, int( 174 - tempValL));

	g.setGradientFill(ColourGradient(Colours::red,
		0.0f, 0.0f,
		Colours::green,
		0.0f, 100.0f,
		false));
	g.fillRect(125, int(15 + tempValR), 30, int(174 - tempValR));
    //[/UserPaint]
}

void MeterComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    curLeft->setBounds (24, 216, 40, 16);
    curRight->setBounds (104, 216, 40, 16);
    peakLeft->setBounds (24, 240, 40, 16);
    peakRight->setBounds (104, 240, 40, 16);
    RMSLeft->setBounds (24, 264, 40, 16);
    RMSRight->setBounds (104, 264, 40, 16);
    curText->setBounds (64, 216, 40, 16);
    peakText->setBounds (64, 240, 40, 16);
    rmsText->setBounds (64, 264, 40, 16);
    ledPeakMeterChild->setBounds (56, 12, 56, 180);
    inSlider->setBounds (168, 8, 48, 104);
    outSlider->setBounds (168, 120, 48, 104);
    Inlabel->setBounds (160, 8, 23, 24);
    OutLabel->setBounds (160, 120, 32, 24);
    toggleButton->setBounds (156, 232, 56, 48);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MeterComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == inSlider)
    {
        //[UserSliderCode_inSlider] -- add your slider handling code here..
      processor.setIngain(static_cast<float>(sliderThatWasMoved->getValue()));
        //[/UserSliderCode_inSlider]
    }
    else if (sliderThatWasMoved == outSlider)
    {
        //[UserSliderCode_outSlider] -- add your slider handling code here..
      processor.setOutgain(static_cast<float>(sliderThatWasMoved->getValue()));
        //[/UserSliderCode_outSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void MeterComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == toggleButton)
    {
        //[UserButtonCode_toggleButton] -- add your button handler code here..
      processor.setBypass(!processor.getBypass());
        //[/UserButtonCode_toggleButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void MeterComponent::peakFollower(std::vector<float> newValues, double sampleRate, int blockSize)
{
	//Keep them bare values for the text boxes,
	//they need to be accurate

	//Simple peak follower code by Phil Burk via http://www.musicdsp.org/
	//Works on normalized floats

	float halfLife = .04f; //Half life of meter decay in seconds, no idea yet for a good value...
	float scalar = pow<float>(0.5f, 1.f / (halfLife * static_cast<float>(sampleRate) / static_cast<float>(blockSize)));
	float input, output;

	for (unsigned int i = 0; i < 2u; i++)
	{
		//CURRENT - AMPLITUDE

		//Keep them bare values for the text boxes,
		//they need to be accurate
		this->bareValues[i] = newValues[i];

		input = newValues[i];
		output = this->decayingValues[i];

		if (input >= output)
		{
			/* When we hit a peak, ride the peak to the top. */
			output = input;
		}
		else
		{
			/* Exponential decay of output when signal is low. */
			output = output * scalar;
			/*
			** When current gets close to 0.0, set current to 0.0 to prevent FP underflow
			** which can cause a severe performance degradation due to a flood
			** of interrupts.
			*/
			if (output < 0.0000001f) output = 0.0000001f;
		}
		this->decayingValues[i] = output;
	}
	for (unsigned int i = 0; i < 2u; i++)
	{
		//PEAK - AMPLITUDE

		//Keep them bare values for the text boxes,
		//they need to be accurate
		this->bareValues[i+2] = newValues[i];

		// No exponential decay for PEAK-Meter, since only
		// Peak Value is desired
		input = newValues[i];
		if (input >= this->decayingValues[i + 2])
		{
			this->decayingValues[i + 2] = input;
		}
	}
	for (unsigned int i = 0; i < 2u; i++)
	{
		//RMS - AMPLITUDE

		//Keep them bare values for the text boxes,
		//they need to be accurate
		this->bareValues[i+4] = newValues[i+2];

		input = newValues[i+2];
		output = this->decayingValues[i+4];

		if (input >= output)
		{
			/* When we hit a peak, ride the peak to the top. */
			output = input;
		}
		else
		{
			/* Exponential decay of output when signal is low. */
			output = output * scalar;
			/*
			** When current gets close to 0.0, set current to 0.0 to prevent FP underflow
			** which can cause a severe performance degradation due to a flood
			** of interrupts.
			*/
			if (output < 0.0000001f) output = 0.0000001f;
		}
		this->decayingValues[i+4] = output;
	}
}


void MeterComponent::setValue(std::vector<float> meterValues, double sampleRate, int blockSize)
{
	// Get exponential decay through peak follower
	// Will set values from vector meterValues to
	// internal vector of values, so you dont need
	// to worry about that my friend :)
	this->peakFollower(meterValues, sampleRate, blockSize);

	//Set the text boxes, decibel readings
	this->curLeft->setText(  (juce::String(aux::linearToDecibel(bareValues[0])     , 2)).substring(0, 4) + " dB", juce::dontSendNotification);
	this->curRight->setText( (juce::String(aux::linearToDecibel(bareValues[1])     , 2)).substring(0,4) + " dB", juce::dontSendNotification);
	this->peakLeft->setText( (juce::String(aux::linearToDecibel(decayingValues[2]) , 1)).substring(0, 4) + " dB", juce::dontSendNotification);
	this->peakRight->setText((juce::String(aux::linearToDecibel(decayingValues[3]) , 1)).substring(0, 4) + " dB", juce::dontSendNotification);
	this->RMSLeft->setText(  (juce::String(aux::linearToDecibel(bareValues[4])     , 2)).substring(0, 4) + " dB", juce::dontSendNotification);
	this->RMSRight->setText( (juce::String(aux::linearToDecibel(bareValues[5])     , 2)).substring(0, 4) + " dB", juce::dontSendNotification);

	//Give current values to LED-Meters
	ledPeakMeterChild->setLValue(decayingValues[1]);
	ledPeakMeterChild->setRValue(decayingValues[0]);

	//If clipped, paint box of value in red
	if (bareValues[0] > 1.f)
	{
		curLeft->setColour(Label::backgroundColourId, Colours::red);
	}
	else
	{
		curLeft->setColour(Label::backgroundColourId, Colour(0xff7e7e7e));
	}
	if (bareValues[1] > 1.f)
	{
		curRight->setColour(Label::backgroundColourId, Colours::red);
	}
	else
	{
		curRight->setColour(Label::backgroundColourId, Colour(0xff7e7e7e));
	}
	if (bareValues[2] > 1.f)
	{
		peakLeft->setColour(Label::backgroundColourId, Colours::red);
	}
	if (bareValues[3] > 1.f)
	{
		peakRight->setColour(Label::backgroundColourId, Colours::red);
	}
	if (bareValues[4] > 1.f)
	{
		RMSLeft->setColour(Label::backgroundColourId, Colours::red);
	}
	else
	{
		RMSLeft->setColour(Label::backgroundColourId, Colour(0xff7e7e7e));
	}
	if (bareValues[5] > 1.f)
	{
		RMSRight->setColour(Label::backgroundColourId, Colours::red);
	}
	else
	{
		RMSRight->setColour(Label::backgroundColourId, Colour(0xff7e7e7e));
	}

	this->repaint();
}

void MeterComponent::mouseDown(const MouseEvent& mouseIn)
{
	juce::Point<int> relativeMouseDown;
	//Could not get contains or reallycontains to work,  nevermind it.
	if (relativeMouseDown = (mouseIn.getMouseDownPosition() - peakLeft->getPosition()),
		relativeMouseDown.getX() >= 0 && relativeMouseDown.getX() < peakLeft->getWidth() &&
		relativeMouseDown.getY() >= 0 && relativeMouseDown.getY() < peakLeft->getHeight())
	{
		peakLeft->setColour(Label::backgroundColourId, Colour(0xff7e7e7e));
		bareValues[2] = 0.f;
		decayingValues[2] = 0.f;
	}
	else if (relativeMouseDown = (mouseIn.getMouseDownPosition() - peakRight->getPosition()),
			 relativeMouseDown.getX() >= 0 && relativeMouseDown.getX() < peakRight->getWidth() &&
			 relativeMouseDown.getY() >= 0 && relativeMouseDown.getY() < peakRight->getHeight())
	{
		peakRight->setColour(Label::backgroundColourId, Colour(0xff7e7e7e));
		bareValues[3] = 0.f;
		decayingValues[3] = 0.f;
	}
	else if (relativeMouseDown = (mouseIn.getMouseDownPosition() - peakText->getPosition()),
			 relativeMouseDown.getX() >= 0 && relativeMouseDown.getX() < peakText->getWidth() &&
			 relativeMouseDown.getY() >= 0 && relativeMouseDown.getY() < peakText->getHeight())
	{
		peakRight->setColour(Label::backgroundColourId, Colour(0xff7e7e7e));
		peakLeft->setColour(Label::backgroundColourId, Colour(0xff7e7e7e));
		bareValues[2] = 0.f;
		bareValues[3] = 0.f;
		decayingValues[2] = 0.f;
		decayingValues[3] = 0.f;
	}
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MeterComponent" componentName="meterChild"
                 parentClasses="public Component" constructorParams="BlankenhainAudioProcessor&amp; p"
                 variableInitialisers="processor(p)" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="1" initialWidth="220"
                 initialHeight="300">
  <BACKGROUND backgroundColour="ff000000">
    <RECT pos="120 10 40 182" fill="solid: ff000000" hasStroke="1" stroke="5, mitered, butt"
          strokeColour="solid: ffffffff"/>
    <RECT pos="8 10 40 182" fill="solid: ff000000" hasStroke="1" stroke="5, mitered, butt"
          strokeColour="solid: ffffffff"/>
  </BACKGROUND>
  <LABEL name="curLeft" id="e44918e7c6b44efc" memberName="curLeft" virtualName=""
         explicitFocusOrder="0" pos="24 216 40 16" bkgCol="ff7e7e7e" edTextCol="ff000000"
         edBkgCol="0" labelText="? dB" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12"
         bold="0" italic="0" justification="36"/>
  <LABEL name="curRight" id="40907e8926b60d92" memberName="curRight" virtualName=""
         explicitFocusOrder="0" pos="104 216 40 16" bkgCol="ff7e7e7e"
         edTextCol="ff000000" edBkgCol="0" labelText="? dB" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12" bold="0" italic="0" justification="36"/>
  <LABEL name="peakLeft" id="fb5189acf29f5db8" memberName="peakLeft" virtualName=""
         explicitFocusOrder="0" pos="24 240 40 16" bkgCol="ff7e7e7e" edTextCol="ff000000"
         edBkgCol="0" labelText="? dB" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12"
         bold="0" italic="0" justification="36"/>
  <LABEL name="peakRight" id="d7f00102b0c6cb6b" memberName="peakRight"
         virtualName="" explicitFocusOrder="0" pos="104 240 40 16" bkgCol="ff7e7e7e"
         edTextCol="ff000000" edBkgCol="0" labelText="? dB" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12" bold="0" italic="0" justification="36"/>
  <LABEL name="RMSLeft" id="31d2229f458b750" memberName="RMSLeft" virtualName=""
         explicitFocusOrder="0" pos="24 264 40 16" bkgCol="ff7e7e7e" edTextCol="ff000000"
         edBkgCol="0" labelText="? dB" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12"
         bold="0" italic="0" justification="36"/>
  <LABEL name="RMSRight" id="b6e6d28afd03ec9" memberName="RMSRight" virtualName=""
         explicitFocusOrder="0" pos="104 264 40 16" bkgCol="ff7e7e7e"
         edTextCol="ff000000" edBkgCol="0" labelText="? dB" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12" bold="0" italic="0" justification="36"/>
  <LABEL name="curText" id="424a38d38b34428b" memberName="curText" virtualName=""
         explicitFocusOrder="0" pos="64 216 40 16" bkgCol="ff000000" textCol="ffffffff"
         outlineCol="ffffffff" edTextCol="ff000000" edBkgCol="0" labelText="cur"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="peakText" id="f3dea9733105c26d" memberName="peakText" virtualName=""
         explicitFocusOrder="0" pos="64 240 40 16" bkgCol="ff000000" textCol="ffffffff"
         outlineCol="ffffffff" edTextCol="ff000000" edBkgCol="0" labelText="Peak"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="rmsText" id="643eeda53297a283" memberName="rmsText" virtualName=""
         explicitFocusOrder="0" pos="64 264 40 16" bkgCol="ff000000" textCol="ffffffff"
         outlineCol="ffffffff" edTextCol="ff000000" edBkgCol="0" labelText="RMS"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="36"/>
  <JUCERCOMP name="ledPeakMeterChild" id="be4156e19e2686ec" memberName="ledPeakMeterChild"
             virtualName="" explicitFocusOrder="0" pos="56 12 56 180" sourceFile="ledMeterComponent.cpp"
             constructorParams=""/>
  <SLIDER name="inSlider" id="7b7c7fed83036044" memberName="inSlider" virtualName=""
          explicitFocusOrder="0" pos="168 8 48 104" min="-12" max="12"
          int="0" style="LinearVertical" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="outSlider" id="1a9cecb548ca24d6" memberName="outSlider"
          virtualName="" explicitFocusOrder="0" pos="168 120 48 104" min="-12"
          max="12" int="0" style="LinearVertical" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="Inlabel" id="7368dae04b28cd61" memberName="Inlabel" virtualName=""
         explicitFocusOrder="0" pos="160 8 23 24" textCol="ffffffff" edTextCol="ff000000"
         edBkgCol="0" labelText="In" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="OutLabel" id="188d50d159a1bd3c" memberName="OutLabel" virtualName=""
         explicitFocusOrder="0" pos="160 120 32 24" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="Out" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="new toggle button" id="f24c5edb5f9dfec4" memberName="toggleButton"
                virtualName="" explicitFocusOrder="0" pos="156 232 56 48" txtcol="ffffffff"
                buttonText="bypass" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.2.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "WittersrodaAudioProcessorEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
WittersrodaAudioProcessorEditor::WittersrodaAudioProcessorEditor (WittersrodaAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (component = new AdsrComponent());
    component->setName ("new component");

    addAndMakeVisible (component2 = new LfoComponent());
    component2->setName ("new component");

    addAndMakeVisible (component3 = new LfoComponent());
    component3->setName ("new component");

    addAndMakeVisible (component4 = new AdsrComponent());
    component4->setName ("new component");

    addAndMakeVisible (component5 = new WaveGenerationComponent());
    component5->setName ("new component");

    addAndMakeVisible (component6 = new WaveGenerationComponent());
    component6->setName ("new component");

    addAndMakeVisible (component7 = new WaveVerschwurbelungComponent());
    component7->setName ("new component");


    //[UserPreSize]
    //[/UserPreSize]

    setSize (800, 600);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

WittersrodaAudioProcessorEditor::~WittersrodaAudioProcessorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    component = nullptr;
    component2 = nullptr;
    component3 = nullptr;
    component4 = nullptr;
    component5 = nullptr;
    component6 = nullptr;
    component7 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void WittersrodaAudioProcessorEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffdeea12));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void WittersrodaAudioProcessorEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    component->setBounds (proportionOfWidth (0.3558f), proportionOfHeight (0.5426f), proportionOfWidth (0.3024f), proportionOfHeight (0.2016f));
    component2->setBounds (proportionOfWidth (0.0323f), proportionOfHeight (0.5607f), proportionOfWidth (0.3024f), proportionOfHeight (0.4083f));
    component3->setBounds (proportionOfWidth (0.6723f), proportionOfHeight (0.5607f), proportionOfWidth (0.3024f), proportionOfHeight (0.3979f));
    component4->setBounds (proportionOfWidth (0.3488f), proportionOfHeight (0.7468f), proportionOfWidth (0.3094f), proportionOfHeight (0.2248f));
    component5->setBounds (proportionOfWidth (0.0113f), proportionOfHeight (0.0207f), proportionOfWidth (0.3263f), proportionOfHeight (0.4755f));
    component6->setBounds (proportionOfWidth (0.6639f), proportionOfHeight (0.0207f), proportionOfWidth (0.3263f), proportionOfHeight (0.4961f));
    component7->setBounds (proportionOfWidth (0.3488f), proportionOfHeight (0.0207f), proportionOfWidth (0.3094f), proportionOfHeight (0.4961f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void WittersrodaAudioProcessorEditor::timerCallback()
{
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="WittersrodaAudioProcessorEditor"
                 componentName="" parentClasses="public AudioProcessorEditor, public Timer"
                 constructorParams="WittersrodaAudioProcessor&amp; p" variableInitialisers="AudioProcessorEditor(&amp;p), processor(p)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="800" initialHeight="600">
  <BACKGROUND backgroundColour="ffdeea12"/>
  <GENERICCOMPONENT name="new component" id="c6a4da1e3dd7ac75" memberName="component"
                    virtualName="" explicitFocusOrder="0" pos="35.584% 54.264% 30.239% 20.155%"
                    class="AdsrComponent" params=""/>
  <GENERICCOMPONENT name="new component" id="c5e7617bd68c1089" memberName="component2"
                    virtualName="" explicitFocusOrder="0" pos="3.235% 56.072% 30.239% 40.827%"
                    class="LfoComponent" params=""/>
  <GENERICCOMPONENT name="new component" id="e7ca55d0064c6237" memberName="component3"
                    virtualName="" explicitFocusOrder="0" pos="67.229% 56.072% 30.239% 39.793%"
                    class="LfoComponent" params=""/>
  <GENERICCOMPONENT name="new component" id="f3a0b4f4a89f4dfd" memberName="component4"
                    virtualName="" explicitFocusOrder="0" pos="34.88% 74.677% 30.942% 22.481%"
                    class="AdsrComponent" params=""/>
  <GENERICCOMPONENT name="new component" id="5b1df9b36aab1ca0" memberName="component5"
                    virtualName="" explicitFocusOrder="0" pos="1.125% 2.067% 32.63% 47.545%"
                    class="WaveGenerationComponent" params=""/>
  <GENERICCOMPONENT name="new component" id="a9466d85c50515d2" memberName="component6"
                    virtualName="" explicitFocusOrder="0" pos="66.385% 2.067% 32.63% 49.612%"
                    class="WaveGenerationComponent" params=""/>
  <GENERICCOMPONENT name="new component" id="aefffa76c5d6e74c" memberName="component7"
                    virtualName="" explicitFocusOrder="0" pos="34.88% 2.067% 30.942% 49.612%"
                    class="WaveVerschwurbelungComponent" params=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

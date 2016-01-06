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
#ifdef PLUGIN_WITTERSRODA
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

    addAndMakeVisible (component8 = new ModulationMatrixComponent());
    component8->setName ("new component");


    //[UserPreSize]
    //[/UserPreSize]

    setSize (900, 600);


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
    component8 = nullptr;


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

    component->setBounds (proportionOfWidth (0.2535f), proportionOfHeight (0.4574f), proportionOfWidth (0.3027f), proportionOfHeight (0.2503f));
    component2->setBounds (proportionOfWidth (0.0058f), proportionOfHeight (0.5092f), proportionOfWidth (0.2410f), proportionOfHeight (0.4660f));
    component3->setBounds (proportionOfWidth (0.5605f), proportionOfHeight (0.5264f), proportionOfWidth (0.2936f), proportionOfHeight (0.4509f));
    component4->setBounds (proportionOfWidth (0.2535f), proportionOfHeight (0.7163f), proportionOfWidth (0.3002f), proportionOfHeight (0.2600f));
    component5->setBounds (proportionOfWidth (0.0058f), proportionOfHeight (0.0162f), proportionOfWidth (0.2344f), proportionOfHeight (0.4757f));
    component6->setBounds (proportionOfWidth (0.5671f), proportionOfHeight (0.0173f), proportionOfWidth (0.2877f), proportionOfHeight (0.4962f));
    component7->setBounds (proportionOfWidth (0.2469f), proportionOfHeight (0.0173f), proportionOfWidth (0.3103f), proportionOfHeight (0.4229f));
    component8->setBounds (proportionOfWidth (0.8607f), proportionOfHeight (0.0173f), proportionOfWidth (0.1251f), proportionOfHeight (0.9579f));
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
                 fixedSize="0" initialWidth="900" initialHeight="600">
  <BACKGROUND backgroundColour="ffdeea12"/>
  <GENERICCOMPONENT name="new component" id="c6a4da1e3dd7ac75" memberName="component"
                    virtualName="" explicitFocusOrder="0" pos="25.328% 45.693% 30.277% 24.964%"
                    class="AdsrComponent" params=""/>
  <GENERICCOMPONENT name="new component" id="c5e7617bd68c1089" memberName="component2"
                    virtualName="" explicitFocusOrder="0" pos="0.582% 50.949% 24.163% 46.569%"
                    class="LfoComponent" params=""/>
  <GENERICCOMPONENT name="new component" id="e7ca55d0064c6237" memberName="component3"
                    virtualName="" explicitFocusOrder="0" pos="56.041% 52.701% 29.403% 45.109%"
                    class="LfoComponent" params=""/>
  <GENERICCOMPONENT name="new component" id="f3a0b4f4a89f4dfd" memberName="component4"
                    virtualName="" explicitFocusOrder="0" pos="25.328% 71.679% 29.985% 25.985%"
                    class="AdsrComponent" params=""/>
  <GENERICCOMPONENT name="new component" id="5b1df9b36aab1ca0" memberName="component5"
                    virtualName="" explicitFocusOrder="0" pos="0.582% 1.606% 23.435% 47.591%"
                    class="WaveGenerationComponent" params=""/>
  <GENERICCOMPONENT name="new component" id="a9466d85c50515d2" memberName="component6"
                    virtualName="" explicitFocusOrder="0" pos="56.769% 1.752% 28.821% 49.635%"
                    class="WaveGenerationComponent" params=""/>
  <GENERICCOMPONENT name="new component" id="aefffa76c5d6e74c" memberName="component7"
                    virtualName="" explicitFocusOrder="0" pos="24.745% 1.752% 31.004% 42.336%"
                    class="WaveVerschwurbelungComponent" params=""/>
  <GENERICCOMPONENT name="new component" id="7083b3137b49446a" memberName="component8"
                    virtualName="" explicitFocusOrder="0" pos="86.026% 1.752% 12.518% 95.766%"
                    class="ModulationMatrixComponent" params=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
#endif
//[/EndFile]

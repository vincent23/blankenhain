/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 4.0.1

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "MeterComponent3.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
ledPeakMeterComponent::ledPeakMeterComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]


    //[UserPreSize]
    //[/UserPreSize]

    setSize (60, 175);


    //[Constructor] You can add your own custom stuff here..
	valueL = 0.f;
	valueR = 0.f;
    //[/Constructor]
}

ledPeakMeterComponent::~ledPeakMeterComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void ledPeakMeterComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.setColour (Colour (0x30a52a9f));
    g.fillRect (proportionOfWidth (0.0667f), proportionOfHeight (0.9371f), proportionOfWidth (0.3333f), proportionOfHeight (0.0457f));

    g.setColour (Colour (0x30a52a9f));
    g.fillRect (proportionOfWidth (0.0667f), proportionOfHeight (0.8457f), proportionOfWidth (0.3333f), proportionOfHeight (0.0457f));

    g.setColour (Colour (0x30a52a9f));
    g.fillRect (proportionOfWidth (0.0667f), proportionOfHeight (0.7543f), proportionOfWidth (0.3333f), proportionOfHeight (0.0457f));

    g.setColour (Colour (0x30a52a9f));
    g.fillRect (proportionOfWidth (0.0667f), proportionOfHeight (0.6629f), proportionOfWidth (0.3333f), proportionOfHeight (0.0457f));

    g.setColour (Colour (0x30a52a9f));
    g.fillRect (proportionOfWidth (0.0667f), proportionOfHeight (0.5714f), proportionOfWidth (0.3333f), proportionOfHeight (0.0457f));

    g.setColour (Colour (0x30ff2a9f));
    g.fillRect (proportionOfWidth (0.0667f), proportionOfHeight (0.4800f), proportionOfWidth (0.3333f), proportionOfHeight (0.0457f));

    g.setColour (Colour (0x30ff2a9f));
    g.fillRect (proportionOfWidth (0.0667f), proportionOfHeight (0.3886f), proportionOfWidth (0.3333f), proportionOfHeight (0.0457f));

    g.setColour (Colour (0x30ff2a9f));
    g.fillRect (proportionOfWidth (0.0667f), proportionOfHeight (0.2971f), proportionOfWidth (0.3333f), proportionOfHeight (0.0457f));

    g.setColour (Colour (0x30ff9c9f));
    g.fillRect (proportionOfWidth (0.0667f), proportionOfHeight (0.2057f), proportionOfWidth (0.3333f), proportionOfHeight (0.0457f));

    g.setColour (Colour (0x30ff9c9f));
    g.fillRect (proportionOfWidth (0.0667f), proportionOfHeight (0.1143f), proportionOfWidth (0.3333f), proportionOfHeight (0.0457f));

    g.setColour (Colour (0x30a52a9f));
    g.fillRect (proportionOfWidth (0.6000f), proportionOfHeight (0.9371f), proportionOfWidth (0.3333f), proportionOfHeight (0.0457f));

    g.setColour (Colour (0x30a52a9f));
    g.fillRect (proportionOfWidth (0.6000f), proportionOfHeight (0.8457f), proportionOfWidth (0.3333f), proportionOfHeight (0.0457f));

    g.setColour (Colour (0x30a52a9f));
    g.fillRect (proportionOfWidth (0.6000f), proportionOfHeight (0.7543f), proportionOfWidth (0.3333f), proportionOfHeight (0.0457f));

    g.setColour (Colour (0x30a52a9f));
    g.fillRect (proportionOfWidth (0.6000f), proportionOfHeight (0.6629f), proportionOfWidth (0.3333f), proportionOfHeight (0.0457f));

    g.setColour (Colour (0x30a52a9f));
    g.fillRect (proportionOfWidth (0.6000f), proportionOfHeight (0.5714f), proportionOfWidth (0.3333f), proportionOfHeight (0.0457f));

    g.setColour (Colour (0x30ff2a9f));
    g.fillRect (proportionOfWidth (0.6000f), proportionOfHeight (0.4800f), proportionOfWidth (0.3333f), proportionOfHeight (0.0457f));

    g.setColour (Colour (0x30ff2a9f));
    g.fillRect (proportionOfWidth (0.6000f), proportionOfHeight (0.3886f), proportionOfWidth (0.3333f), proportionOfHeight (0.0457f));

    g.setColour (Colour (0x30ff2a9f));
    g.fillRect (proportionOfWidth (0.6000f), proportionOfHeight (0.2971f), proportionOfWidth (0.3333f), proportionOfHeight (0.0457f));

    g.setColour (Colour (0x30ff9c9f));
    g.fillRect (proportionOfWidth (0.6000f), proportionOfHeight (0.2057f), proportionOfWidth (0.3333f), proportionOfHeight (0.0457f));

    g.setColour (Colour (0x30ff9c9f));
    g.fillRect (proportionOfWidth (0.6000f), proportionOfHeight (0.1143f), proportionOfWidth (0.3333f), proportionOfHeight (0.0457f));

    g.setColour (Colour (0x30ffcd9f));
    g.fillRect (proportionOfWidth (0.6000f), proportionOfHeight (0.0229f), proportionOfWidth (0.3333f), proportionOfHeight (0.0457f));

    g.setColour (Colour (0x30ffcd9f));
    g.fillRect (proportionOfWidth (0.0667f), proportionOfHeight (0.0229f), proportionOfWidth (0.3333f), proportionOfHeight (0.0457f));

    //[UserPaint] Add your own custom painting code here..

	if (valueL > 0.1f)
	{
		if (valueL > 0.2f)
		{
			g.setColour(Colour(0xffa52a9f));
			g.fillRect(proportionOfWidth(0.6000f), proportionOfHeight(0.7543f), proportionOfWidth(0.3333f), proportionOfHeight(0.0457f));
			if (valueL > 0.3f)
			{
				g.setColour(Colour(0xffa52a9f));
				g.fillRect(proportionOfWidth(0.6000f), proportionOfHeight(0.6629f), proportionOfWidth(0.3333f), proportionOfHeight(0.0457f));
				if (valueL > 0.4f)
				{
					g.setColour(Colour(0xffa52a9f));
					g.fillRect(proportionOfWidth(0.6000f), proportionOfHeight(0.5714f), proportionOfWidth(0.3333f), proportionOfHeight(0.0457f));
					if (valueL > 0.5f)
					{
						g.setColour(Colour(0xffff2a9f));
						g.fillRect(proportionOfWidth(0.6000f), proportionOfHeight(0.4800f), proportionOfWidth(0.3333f), proportionOfHeight(0.0457f));
						if (valueL > 0.6f)
						{
							g.setColour(Colour(0xffff2a9f));
							g.fillRect(proportionOfWidth(0.6000f), proportionOfHeight(0.3886f), proportionOfWidth(0.3333f), proportionOfHeight(0.0457f));
							if (valueL > 0.7f)
							{
								g.setColour(Colour(0xffff2a9f));
								g.fillRect(proportionOfWidth(0.6000f), proportionOfHeight(0.2971f), proportionOfWidth(0.3333f), proportionOfHeight(0.0457f));
								if (valueL > 0.8f)
								{
									g.setColour(Colour(0xffff9c9f));
									g.fillRect(proportionOfWidth(0.6000f), proportionOfHeight(0.2057f), proportionOfWidth(0.3333f), proportionOfHeight(0.0457f));
									if (valueL > 0.9f)
									{
										g.setColour(Colour(0xffff9c9f));
										g.fillRect(proportionOfWidth(0.6000f), proportionOfHeight(0.1143f), proportionOfWidth(0.3333f), proportionOfHeight(0.0457f));
										if (valueL > 0.9f)
										{
											g.setColour(Colour(0xffffcd9f));
											g.fillRect(proportionOfWidth(0.6000f), proportionOfHeight(0.0229f), proportionOfWidth(0.3333f), proportionOfHeight(0.0457f));

										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
    //[/UserPaint]
}

void ledPeakMeterComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void ledPeakMeterComponent::setRValue(float in)
{
	valueR = in;
	this->repaint();
}

float ledPeakMeterComponent::getRValue()
{
	return valueR;
}

void ledPeakMeterComponent::setLValue(float in)
{
	valueL = in;
	this->repaint();
}

float ledPeakMeterComponent::getLValue()
{
	return valueL;
}


//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ledPeakMeterComponent" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="60" initialHeight="175">
  <BACKGROUND backgroundColour="0">
    <RECT pos="6.667% 93.714% 33.333% 4.571%" fill="solid: 30a52a9f" hasStroke="0"/>
    <RECT pos="6.667% 84.571% 33.333% 4.571%" fill="solid: 30a52a9f" hasStroke="0"/>
    <RECT pos="6.667% 75.429% 33.333% 4.571%" fill="solid: 30a52a9f" hasStroke="0"/>
    <RECT pos="6.667% 66.286% 33.333% 4.571%" fill="solid: 30a52a9f" hasStroke="0"/>
    <RECT pos="6.667% 57.143% 33.333% 4.571%" fill="solid: 30a52a9f" hasStroke="0"/>
    <RECT pos="6.667% 48% 33.333% 4.571%" fill="solid: 30ff2a9f" hasStroke="0"/>
    <RECT pos="6.667% 38.857% 33.333% 4.571%" fill="solid: 30ff2a9f" hasStroke="0"/>
    <RECT pos="6.667% 29.714% 33.333% 4.571%" fill="solid: 30ff2a9f" hasStroke="0"/>
    <RECT pos="6.667% 20.571% 33.333% 4.571%" fill="solid: 30ff9c9f" hasStroke="0"/>
    <RECT pos="6.667% 11.429% 33.333% 4.571%" fill="solid: 30ff9c9f" hasStroke="0"/>
    <RECT pos="60% 93.714% 33.333% 4.571%" fill="solid: 30a52a9f" hasStroke="0"/>
    <RECT pos="60% 84.571% 33.333% 4.571%" fill="solid: 30a52a9f" hasStroke="0"/>
    <RECT pos="60% 75.429% 33.333% 4.571%" fill="solid: 30a52a9f" hasStroke="0"/>
    <RECT pos="60% 66.286% 33.333% 4.571%" fill="solid: 30a52a9f" hasStroke="0"/>
    <RECT pos="60% 57.143% 33.333% 4.571%" fill="solid: 30a52a9f" hasStroke="0"/>
    <RECT pos="60% 48% 33.333% 4.571%" fill="solid: 30ff2a9f" hasStroke="0"/>
    <RECT pos="60% 38.857% 33.333% 4.571%" fill="solid: 30ff2a9f" hasStroke="0"/>
    <RECT pos="60% 29.714% 33.333% 4.571%" fill="solid: 30ff2a9f" hasStroke="0"/>
    <RECT pos="60% 20.571% 33.333% 4.571%" fill="solid: 30ff9c9f" hasStroke="0"/>
    <RECT pos="60% 11.429% 33.333% 4.571%" fill="solid: 30ff9c9f" hasStroke="0"/>
    <RECT pos="60% 2.286% 33.333% 4.571%" fill="solid: 30ffcd9f" hasStroke="0"/>
    <RECT pos="6.667% 2.286% 33.333% 4.571%" fill="solid: 30ffcd9f" hasStroke="0"/>
  </BACKGROUND>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

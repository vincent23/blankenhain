/*
  ==============================================================================

    VolumeAudioProcessor.h
    Created: 2 Nov 2015 10:44:30am
    Author:  Dusti

  ==============================================================================
*/

#ifndef VOLUMEAUDIOPROCESSOR_H_INCLUDED
#define VOLUMEAUDIOPROCESSOR_H_INCLUDED

#include <juce>
#include "BlankenhainAudioProcessor.h"
#include "FloatParameter.h"
#include "BoolParameter.h"


class VolumeAudioProcessor : public BlankenhainAudioProcessor
{
public:
	VolumeAudioProcessor();

	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

	void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

	AudioProcessorEditor* createEditor() override;

	void setVolumeL(FloatParameter volumeL);
	void setVolumeR(FloatParameter volumeR);
	void setStereoCoupling(bool stereoCoupling);
	void switchStereoCoupling();
	float getVolumeL();
	float getVolumeR();
	bool getStereoCoupling();

protected:
	var getState() override;
	void setState(const var& state) override;

private:
	FloatParameter *volumeL;
	FloatParameter *volumeR;
	BoolParameter *stereoCoupling;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VolumeAudioProcessor)
};


#endif  // VOLUMEAUDIOPROCESSOR_H_INCLUDED

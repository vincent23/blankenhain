#include "pitchshiftMidiPlugin.h"
#include "pitchshiftMidiEffect.h"

pitchshiftMidiPlugin::pitchshiftMidiPlugin(audioMasterCallback audioMaster)
	: MidiPluginBase(audioMaster, new pitchshiftMidiEffect)
{
	setUniqueID('bhPS');
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	// Change name here
	return new pitchshiftMidiPlugin(audioMaster);
}
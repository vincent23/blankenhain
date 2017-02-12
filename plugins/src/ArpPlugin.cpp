#include "ArpPlugin.h"
#include "ArpMidiEffect.h"

ArpPlugin::ArpPlugin(audioMasterCallback audioMaster)
	: MidiPluginBase(audioMaster, new ArpMidiEffect)
{
	setUniqueID('bhAR');
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	// Change name here
	return new ArpPlugin(audioMaster);
}
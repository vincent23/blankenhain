#include "PanPlugin.h"

#include "PanEffect.h"

PanPlugin::PanPlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new PanEffect)
{ }

void PanPlugin::open()
{
	setUniqueID(1991);
	isSynth(false);
}

void PanPlugin::close()
{
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new PanPlugin(audioMaster);
}
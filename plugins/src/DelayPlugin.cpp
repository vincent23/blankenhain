#include "DelayPlugin.h"

#include "DelayEffect.h"

DelayPlugin::DelayPlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new DelayEffect)
{ }

void DelayPlugin::open()
{
	setUniqueID(1991);
	isSynth(false);
}

void DelayPlugin::close()
{
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new DelayPlugin(audioMaster);
}
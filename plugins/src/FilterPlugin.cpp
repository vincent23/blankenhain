#include "FilterPlugin.h"

#include "FilterEffect.h"

FilterPlugin::FilterPlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new FilterEffect)
{ }

void FilterPlugin::open()
{
	setUniqueID(1993);
	isSynth(false);
}

void FilterPlugin::close()
{
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new FilterPlugin(audioMaster);
}
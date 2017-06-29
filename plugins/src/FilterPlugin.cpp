#include "FilterPlugin.h"

#include "FilterEffect.h"

FilterPlugin::FilterPlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new FilterEffect)
{
	setUniqueID('bhFi');
	setEditor(new FilterPluginEditor(this));
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new FilterPlugin(audioMaster);
}
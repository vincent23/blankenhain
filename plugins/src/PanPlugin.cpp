// Change header here
#include "PanPlugin.h"

PanPlugin::PanPlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new PanEffect)
{
	setUniqueID('bhWd');
	setEditor(new PanPluginEditor(this));
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new PanPlugin(audioMaster);
}
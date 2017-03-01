// Change header here
#include "WidthPlugin.h"

WidthPlugin::WidthPlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new WidthEffect)
{
	setUniqueID('bhWd');
	setEditor(new WidthPluginEditor(this));
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new WidthPlugin(audioMaster);
}
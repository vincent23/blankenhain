#include "NoiseGatePlugin.h"

NoiseGatePlugin::NoiseGatePlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new NoiseGateEffect)
{
	setUniqueID('bhNg');
	//setEditor(new NoiseGatePluginEditor(this));
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new NoiseGatePlugin(audioMaster);
}
// Change header here
#include "InversePhasePlugin.h"
#include "InversePhaseEffect.h"

InversePhasePlugin::InversePhasePlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new InversePhaseEffect)
{
	setUniqueID('bhIP');
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new InversePhasePlugin(audioMaster);
}
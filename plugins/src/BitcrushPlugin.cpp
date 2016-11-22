#include "BitcrushPlugin.h"

#include "BitcrushEffect.h"

BitcrushPlugin::BitcrushPlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new BitcrushEffect)
{
	setUniqueID('bhBC');
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new BitcrushPlugin(audioMaster);
}
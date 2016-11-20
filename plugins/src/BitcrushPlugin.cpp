#include "BitcrushPlugin.h"

#include "BitcrushEffect.h"

BitcrushPlugin::BitcrushPlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new BitcrushEffect)
{ }

void BitcrushPlugin::open()
{
	setUniqueID(189);
	isSynth(false);
}

void BitcrushPlugin::close()
{

}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new BitcrushPlugin(audioMaster);
}
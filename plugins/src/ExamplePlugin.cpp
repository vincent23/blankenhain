// Change header here
#include "ExamplePlugin.h"

#include "ExampleEffect.h"

ExamplePlugin::ExamplePlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new ExampleEffect)
{
	setUniqueID('bhEx');
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	// Change name here
	return new ExamplePlugin(audioMaster);
}
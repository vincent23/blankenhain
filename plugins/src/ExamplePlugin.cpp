// Change header here
#include "ExamplePlugin.h"

#include "ExampleEffect.h"

ExamplePlugin::ExamplePlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new ExampleEffect)
{ }

void ExamplePlugin::open()
{
	setUniqueID(1993);
	isSynth(false);
}

void ExamplePlugin::close()
{
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	// Change name here
	return new ExamplePlugin(audioMaster);
}
// Change header here
#include "ExamplePlugin.h"

#include "ExampleEffect.h"
#include "ExamplePluginEditor.h"

ExamplePlugin::ExamplePlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new ExampleEffect)
{
	setUniqueID('bhEx');
	setEditor(new ExamplePluginEditor(this));
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	// Change name here
	return new ExamplePlugin(audioMaster);
}
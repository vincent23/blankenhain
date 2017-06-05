// Change header here
#include "ExamplePlugin.h"

#include "ExampleEffect.h"
#include "ExamplePluginEditor.h"

ExamplePlugin::ExamplePlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new /*Change Name to libblankenhain effect here*/ ExampleEffect)
{
	// Change these four letters to something unique
	setUniqueID('bhEx');
	// Call this if you want to provide a custom imGUI for your plugin
	setEditor(new ExamplePluginEditor(this));
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	// Change name here
	return new ExamplePlugin(audioMaster);
}
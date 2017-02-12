#include "DelayPlugin.h"

#include "DelayEffect.h"

DelayPlugin::DelayPlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new DelayEffect)
{
	setUniqueID('bhDe');
	setEditor(new DelayPluginEditor(this));

}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new DelayPlugin(audioMaster);
}
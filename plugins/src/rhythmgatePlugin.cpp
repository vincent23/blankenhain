#include "RhythmgatePlugin.h"

#include "RhythmgateEffect.h"
#include "RhythmgatePluginEditor.h"

RhythmgatePlugin::RhythmgatePlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new RhythmgateEffect)
{
	setUniqueID('bhRg');
	setEditor(new RhythmgatePluginEditor(this));
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new RhythmgatePlugin(audioMaster);
}
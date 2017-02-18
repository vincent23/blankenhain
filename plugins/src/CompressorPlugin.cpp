#include "CompressorPlugin.h"

#include "CompressorEffect.h"
#include "CompressorPluginEditor.h"
#include "AuxFunc.h"
#include <algorithm>

CompressorPlugin::CompressorPlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new CompressorEffect)
{
	setUniqueID('bhCo');
	setEditor(new CompressorPluginEditor(this));
	currentEnvelope = 0.f;
}

float CompressorPlugin::getEnvelope()
{
	return currentEnvelope;
}

void CompressorPlugin::onAfterProcess()
{
	Sample envelope = static_cast<CompressorEffect*>(effect)->getCurrentEnvelope();
	currentEnvelope = (float)aux::linearToDecibel(static_cast<float>(envelope.maxValue()));
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new CompressorPlugin(audioMaster);
}
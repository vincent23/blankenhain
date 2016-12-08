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
	alignas(16) double lr[2];
	static_cast<CompressorEffect*>(effect)->envelope.getCurrentEnvelope().store_aligned(lr);
	currentEnvelope = (float)aux::linearToDecibel(std::max(lr[0], lr[1]));
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new CompressorPlugin(audioMaster);
}
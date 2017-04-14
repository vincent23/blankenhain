#include "LimiterPlugin.h"
		
#include "LimiterEffect.h"
#include "AuxFunc.h"
#include <algorithm>

LimiterPlugin::LimiterPlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new LimiterEffect)
{
	setUniqueID('bhLi');
	currentEnvelope = 0.f;
}

float LimiterPlugin::getEnvelope()
{
	return currentEnvelope;
}

void LimiterPlugin::onAfterProcess()
{
	Sample envelope = static_cast<LimiterEffect*>(effect)->getCurrentEnvelope();
	currentEnvelope = (float)aux::linearToDecibel(static_cast<float>(envelope.maxValue()));
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new LimiterPlugin(audioMaster);
}
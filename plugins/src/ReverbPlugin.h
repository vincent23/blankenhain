#pragma once
#include "ReverbEffect.h"

#include "PluginBase.h"

class ReverbPlugin : public PluginBase
{
public:
	ReverbPlugin(audioMasterCallback audioMaster);

	virtual void suspend() final
	{
		ReverbEffect* reverb = dynamic_cast<ReverbEffect*>(this->effect);
		if (reverb != nullptr)
		{
			reverb->resetEffect();
		}
	};
};
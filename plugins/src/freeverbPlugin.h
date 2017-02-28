#pragma once
//This is an example for a bh2 plugin

//Include the effectCode here
#include "freeverbEffect.h"
//

// Don't change this
#include "PluginBase.h"
//

// Change the name here
class freeverbPlugin : public PluginBase
{
public:
	freeverbPlugin(audioMasterCallback audioMaster);

	virtual void suspend() final
	{
		freeverbEffect* reverb = dynamic_cast<freeverbEffect*>(this->effect);
		if (reverb != nullptr)
		{
			reverb->resetEffect();
		}
	};
};
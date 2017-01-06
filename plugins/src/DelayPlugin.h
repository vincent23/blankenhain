#pragma once

#include "PluginBase.h"
#include "DelayEffect.h"

class DelayPlugin : public PluginBase
{
public:
  virtual void suspend() final
  {
    DelayEffect* delay = dynamic_cast<DelayEffect*>(this->effect);
    if (delay)
    {
      delay->resetDelayLine();
    }
  };
	DelayPlugin(audioMasterCallback audioMaster);
};
#pragma once

#include "PluginBase.h"
#include "VibratoEffect.h"

class VibratoPlugin : public PluginBase
{
public:
  virtual void suspend() final
  {
    VibratoEffect* vib = dynamic_cast<VibratoEffect*>(this->effect);
    //if (delay)
    //{
    //  delay->resetDelayLine();
    //}
  };
  VibratoPlugin(audioMasterCallback audioMaster);
};
#pragma once

#include "PluginBase.h"
#include "ChorusEffect.h"
#include "ChorusPluginEditor.h"

class ChorusPlugin : public PluginBase
{
public:
  ChorusPlugin(audioMasterCallback audioMaster);
  virtual void suspend() final
  {
	  ChorusEffect* chorus = dynamic_cast<ChorusEffect*>(this->effect);
	  if (chorus)
	  {
		  chorus->resetDelayLine();
	  }
  };
};
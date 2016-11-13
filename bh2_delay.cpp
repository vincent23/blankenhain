#pragma once
#ifdef _BH2_delay

#include "bh2_base.h"
#include "bh2_delay.h"

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
  return new BH2_delay(audioMaster);
}

#endif
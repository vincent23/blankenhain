#pragma once
#ifdef _BH2_pan

#include "bh2_base.h"
#include "bh2_pan.h"

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
  return new BH2_pan(audioMaster);
}

#endif
#pragma once
#ifdef _BH2_filter
#include "bh2_base.h"
// Change header here
#include "bh2_filter.h"

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
  // Change name here
  return new BH2_filter(audioMaster);
}

#endif
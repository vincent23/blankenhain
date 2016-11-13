#pragma once
#ifdef _BH2_bitcrush

#include "bh2_base.h"
#include "bh2_bitcrush.h"


AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
  return new BH2_bitcrush(audioMaster);
}
#endif
#pragma once
#ifdef _BH2_distortion

#include "bh2_base.h"
#include "bh2_distortion.h"


AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
  return new BH2_distortion(audioMaster);
}
#endif
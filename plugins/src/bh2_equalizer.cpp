#pragma once

#include "bh2_base.h"
#include "bh2_equalizer.h"


AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
  return new BH2_equalizer(audioMaster);
}
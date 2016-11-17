#pragma once
#ifdef _BH2_compressor

#include "bh2_base.h"
#include "bh2_compressor.h"

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
  return new BH2_compressor(audioMaster);
}

#endif
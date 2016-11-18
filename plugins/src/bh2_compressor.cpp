#include "bh2_compressor.h"

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new BH2_compressor(audioMaster);
}
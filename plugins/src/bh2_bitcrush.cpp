#include "bh2_bitcrush.h"

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new BH2_bitcrush(audioMaster);
}
#include "bh2_delay.h"

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new BH2_delay(audioMaster);
}
#include "bh2_pan.h"

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new BH2_pan(audioMaster);
}
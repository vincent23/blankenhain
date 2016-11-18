#include "bh2_filter.h"

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	// Change name here
	return new BH2_filter(audioMaster);
}
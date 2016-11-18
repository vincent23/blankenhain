#include "bh2_distortion.h"

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new BH2_distortion(audioMaster);
}
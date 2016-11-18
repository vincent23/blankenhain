#include "bh2_volume.h"

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new BH2_volume(audioMaster);
}
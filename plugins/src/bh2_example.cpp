// Change header here
#include "bh2_example.h"

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	// Change name here
	return new BH2_example(audioMaster);
}
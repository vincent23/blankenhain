#pragma once

#include "PluginBase.h"
#include "VolumeEffect.h"

class VolumePlugin : public PluginBase
{
public:
	VolumePlugin(audioMasterCallback audioMaster);
};
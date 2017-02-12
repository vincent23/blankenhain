#pragma once

#include "PluginBase.h"
#include "VolumeEffect.h"
#include "VolumePluginEditor.h"

class VolumePlugin : public PluginBase
{
public:
	VolumePlugin(audioMasterCallback audioMaster);
};
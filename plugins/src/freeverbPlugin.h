#pragma once
//This is an example for a bh2 plugin

// Don't change this
#include "PluginBase.h"

// Change the name here
class freeverbPlugin : public PluginBase
{
public:
	freeverbPlugin(audioMasterCallback audioMaster);
};
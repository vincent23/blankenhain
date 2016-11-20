#pragma once

#include "PluginBase.h"

class DistortionPlugin : public PluginBase
{
public:
	DistortionPlugin(audioMasterCallback audioMaster);

	void open() override;
	void close() override;
};
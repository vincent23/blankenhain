#pragma once

#include "PluginBase.h"

class EqualizerPlugin : public PluginBase
{
public:
	EqualizerPlugin(audioMasterCallback audioMaster);

	void open() override;
	void close() override;
};

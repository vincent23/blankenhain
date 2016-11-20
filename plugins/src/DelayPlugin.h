#pragma once

#include "PluginBase.h"

class DelayPlugin : public PluginBase
{
public:
	DelayPlugin(audioMasterCallback audioMaster);

	void open() override;
	void close() override;
};
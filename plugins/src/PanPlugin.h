#pragma once

#include "PluginBase.h"

class PanPlugin : public PluginBase
{
public:
	PanPlugin(audioMasterCallback audioMaster);

	void open() override;
	void close() override;
};
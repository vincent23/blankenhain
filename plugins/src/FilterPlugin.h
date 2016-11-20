#pragma once

#include "PluginBase.h"

class FilterPlugin : public PluginBase
{
public:
	FilterPlugin(audioMasterCallback audioMaster);

	void open() override;
	void close() override;
};
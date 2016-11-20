#pragma once

#include "PluginBase.h"

class BitcrushPlugin : public PluginBase
{
public:
	BitcrushPlugin(audioMasterCallback audioMaster);

	void open() override;
	void close() override;
};
#pragma once

#include "PluginBase.h"

class CompressorPlugin : public PluginBase
{
public:
	CompressorPlugin(audioMasterCallback audioMaster);

	void open() override;
	void close() override;
};
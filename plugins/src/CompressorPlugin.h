#pragma once

#include "PluginBase.h"

#include <atomic>

class CompressorPlugin : public PluginBase
{
public:
	CompressorPlugin(audioMasterCallback audioMaster);
	float getEnvelope();

protected:
	void onAfterProcess() override;

private:
	std::atomic<float> currentEnvelope;
};
#pragma once

#include "PluginBase.h"

#include <atomic>

class LimiterPlugin : public PluginBase
{
public:
	LimiterPlugin(audioMasterCallback audioMaster);
	float getEnvelope();

protected:
	void onAfterProcess() override;

private:
	std::atomic<float> currentEnvelope;
};
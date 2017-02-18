#pragma once

#include "ImguiEffectEditor.h"
#include "PluginBase.h"

class EqualizerPluginEditor : public ImguiEffectEditor
{
public:
	EqualizerPluginEditor(PluginBase* plugin);

protected:
	void imguiFrame() override;

private:
	int item = 0;
};
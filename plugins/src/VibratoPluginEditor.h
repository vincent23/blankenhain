#pragma once

#include "ImguiEffectEditor.h"

class VibratoPluginEditor : public ImguiEffectEditor
{
public:
	VibratoPluginEditor(PluginBase* plugin);

protected:
	void imguiFrame() override;
};
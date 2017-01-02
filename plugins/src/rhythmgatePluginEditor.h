#pragma once

#include "ImguiEffectEditor.h"
#include "PluginBase.h"

class RhythmgatePluginEditor : public ImguiEffectEditor
{
public:
	RhythmgatePluginEditor(PluginBase* plugin);

protected:
	void imguiFrame() override;
};
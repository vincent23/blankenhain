#pragma once

#include "ImguiEffectEditor.h"

class ChorusPluginEditor : public ImguiEffectEditor
{
public:
	ChorusPluginEditor(PluginBase* plugin);

protected:
	void imguiFrame() override;
};
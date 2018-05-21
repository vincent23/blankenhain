#pragma once

#include "ImguiEffectEditor.h"
#include "ImguiCustomFunctions.h"

class ChorusPluginEditor : public ImguiEffectEditor
{
public:
	ChorusPluginEditor(PluginBase* plugin);

protected:
	void imguiFrame() override;
};
#pragma once

#include "ImguiEffectEditor.h"
#include "ExamplePlugin.h"

class ExamplePluginEditor : public ImguiEffectEditor
{
public:
	ExamplePluginEditor(PluginBase* plugin);

protected:
	void imguiFrame() override;
};
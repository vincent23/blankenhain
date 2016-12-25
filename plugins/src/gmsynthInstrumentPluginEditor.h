#pragma once

#include "ImguiEffectEditor.h"
#include "PluginBase.h"

class gmsynthPluginEditor : public ImguiEffectEditor
{
public:
	gmsynthPluginEditor(PluginBase* plugin);

protected:
	void imguiFrame() override;
};
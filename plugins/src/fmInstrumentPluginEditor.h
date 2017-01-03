#pragma once

#include "ImguiEffectEditor.h"
#include "PluginBase.h"

class FmInstrumentPluginEditor : public ImguiEffectEditor
{
public:
	FmInstrumentPluginEditor(PluginBase* plugin);

protected:
	void imguiFrame() override;
};
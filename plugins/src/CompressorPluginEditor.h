#pragma once

#include "ImguiEffectEditor.h"

class CompressorPluginEditor : public ImguiEffectEditor
{
public:
	CompressorPluginEditor(PluginBase* plugin);

protected:
	void imguiFrame() override;
};
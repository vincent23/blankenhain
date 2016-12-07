#pragma once

#include "ImguiEffectEditor.h"

class CompressorPluginEditor : public ImguiEffectEditor
{
public:
	CompressorPluginEditor(PluginBase* plugin);

protected:
	void imguiFrame() override;

private:
	// TODO move these to the effect
	float compressorMap(float threshold, float ratio, float knee, float dbIn) const;
	float compressorMapWithMakeup(float threshold, float ratio, float knee, float dbIn) const;
};
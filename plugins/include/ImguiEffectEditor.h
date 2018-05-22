#pragma once

#include <vstgui/plugin-bindings/aeffguieditor.h>
class PluginBase;

#include "ImguiFrameCallback.h"
#include "EffectBase.h"

class ImguiEffectEditor : public AEffGUIEditor, public ImguiFrameCallback {
public:
	ImguiEffectEditor(PluginBase* plugin, int width, int height);

	bool open(void* ptr) override;
	void close() override;
	

	PluginBase& getPlugin();

	EffectBase* getEffectBase();


	void imguiFrame() override;

protected:
	PluginBase& plugin;
};
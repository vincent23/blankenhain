#pragma once

#include "ImguiEffectEditor.h"
#include "PluginBase.h"
#include <vector>

class gmsynthPluginEditor : public ImguiEffectEditor
{
public:
	gmsynthPluginEditor(PluginBase* plugin);
protected:
	void imguiFrame() override;
private:
	std::vector<const char*> instrumentNames;
};
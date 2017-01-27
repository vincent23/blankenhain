#include "VibratoPluginEditor.h"
#include "PluginParameterBundle.h"
#include "PluginBase.h"
#include "AuxFunc.h"
#include "VibratoPlugin.h"
#include "VibratoEffect.h"

#include <algorithm>
#include <string>
#include <imgui.h>

VibratoPluginEditor::VibratoPluginEditor(PluginBase* plugin)
	: ImguiEffectEditor(plugin, 500, 500)
{}


void VibratoPluginEditor::imguiFrame()
{

	ImGui::End();
}
#include "ChorusPluginEditor.h"
#include "PluginParameterBundle.h"
#include "PluginBase.h"
#include "AuxFunc.h"
#include "ChorusPlugin.h"
#include "ChorusEffect.h"

#include <algorithm>
#include <string>
#include <imgui.h>

ChorusPluginEditor::ChorusPluginEditor(PluginBase* plugin)
	: ImguiEffectEditor(plugin, 500, 500)
{}


void ChorusPluginEditor::imguiFrame()
{
	ImGui::End();
}
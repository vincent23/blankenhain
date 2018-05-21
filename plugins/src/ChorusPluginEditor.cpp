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

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);


	renderParam(plugin, 0);

	renderParam(plugin, 1);

	renderParam(plugin, 2);
	renderParam(plugin, 3);
	renderParam(plugin, 4);
	renderParam(plugin, 5);
	renderParam(plugin, 6);
	renderParam(plugin, 7);
	renderParam(plugin, 8);
	renderParam(plugin, 9);

	ImGui::End();
}
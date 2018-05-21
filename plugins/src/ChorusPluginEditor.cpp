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


	const PluginParameterBundle& bundle = plugin.getParameters();
	float tempoSyncOn = bundle.getParameterUnnormalized(10);

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
	renderParam(plugin, 10);
	if (tempoSyncOn == 1.f)
		renderParam(plugin, 11);

	ImGui::End();
}
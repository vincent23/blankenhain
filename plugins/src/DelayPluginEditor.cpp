#include "DelayPluginEditor.h"



DelayPluginEditor::DelayPluginEditor(PluginBase* plugin)
	: ImguiEffectEditor(plugin, 460, 225)
{
}


void DelayPluginEditor::imguiFrame()
{

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);


	renderParam( plugin, 0);

	renderParam( plugin, 1);

	renderParam( plugin, 2);
	renderParam(plugin, 3);
	renderParam(plugin, 4);
	renderParam(plugin, 5);
	renderParam(plugin, 6);

	ImGui::End();
}
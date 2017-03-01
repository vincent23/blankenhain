#include "WidthPluginEditor.h"



WidthPluginEditor::WidthPluginEditor(PluginBase* plugin)
	: ImguiEffectEditor(plugin, 460, 260)
{
}


void WidthPluginEditor::imguiFrame()
{

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);


	renderParam( plugin, 0);

	renderParam( plugin, 1, 0.1f);


	ImGui::Text("Pan LFO");

	renderLFO(plugin, ImVec2(ImGui::GetWindowContentRegionWidth(), 160), 3,2,5,6,8,4,7);

	ImGui::End();
}
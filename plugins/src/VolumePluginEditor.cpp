#include "VolumePluginEditor.h"



VolumePluginEditor::VolumePluginEditor(PluginBase* plugin)
	: ImguiEffectEditor(plugin, 460, 680)
{
}


void VolumePluginEditor::imguiFrame()
{

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);


	renderParam( plugin, 0);

	renderParam( plugin, 1);

	renderParam( plugin, 2);



	ImGui::Text("Detune LFO");

	renderLFO(plugin, ImVec2(ImGui::GetWindowContentRegionWidth(), 160), 4,3,6,7,8,5,9);

	ImGui::End();
}
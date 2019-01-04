#include "VolumePluginEditor.h"



VolumePluginEditor::VolumePluginEditor(PluginBase* plugin)
	: ImguiEffectEditor(plugin, 460, 380)
{
}

void VolumePluginEditor::imguiFrame()
{

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	renderParam(plugin, 0);
	renderParam(plugin, 1);
	renderParam(plugin, 2);

	ImGui::Text("Detune LFO");
	VolumeEffect* effect = dynamic_cast<VolumeEffect*>(this->getEffectBase());

	renderLFO(plugin, effect->getLFO(), effect->getTempoData(), ImVec2(ImGui::GetWindowContentRegionWidth(), 260), 4, 3, 6, 7, 8, 5, 9);

	ImGui::End();
}
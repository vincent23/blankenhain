#include "PolysynthInstrumentPluginEditor.h"


PolysynthInstrumentPluginEditor::PolysynthInstrumentPluginEditor(PluginBase* plugin)
	: ImguiEffectEditor(plugin, 460, 400)
{
}


void PolysynthInstrumentPluginEditor::imguiFrame()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	renderADHSR(plugin, ImVec2(ImGui::GetWindowContentRegionWidth(), 300));

	renderParam(plugin, 8);
	renderParam(plugin, 9);

	ImGui::End();
}
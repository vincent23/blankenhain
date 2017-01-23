#include "glidePolyblepInstrumentPluginEditor.h"



glidePolyblepInstrumentPluginEditor::glidePolyblepInstrumentPluginEditor(PluginBase* plugin)
	: ImguiEffectEditor(plugin, 460, 680)
{
}


void glidePolyblepInstrumentPluginEditor::imguiFrame()
{

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);


	renderADHSR(plugin, ImVec2(ImGui::GetWindowContentRegionWidth(), 300));
	

	renderParam( plugin, 8);

	renderParam( plugin, 9);

	renderParam( plugin, 12);



	ImGui::Text("Detune LFO");

	renderLFO(plugin, ImVec2(ImGui::GetWindowContentRegionWidth(), 160), 11, 10, 14, 15, 16, 13, 17);

	ImGui::End();
}
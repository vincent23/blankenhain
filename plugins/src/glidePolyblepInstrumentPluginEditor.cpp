#include "glidePolyblepInstrumentPluginEditor.h"



glidePolyblepInstrumentPluginEditor::glidePolyblepInstrumentPluginEditor(PluginBase* plugin)
	: ImguiEffectEditor(plugin, 400, 680)
{
}


void glidePolyblepInstrumentPluginEditor::imguiFrame()
{

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	const PluginParameterBundle& bundle = plugin.getParameters();

	renderADHSR(plugin, ImVec2(ImGui::GetWindowContentRegionWidth(), 300));
	
	ImGui::PushID(8);
	renderParam(bundle.getParameter(8), plugin, 8);
	ImGui::PopID();
	ImGui::PushID(9);
	renderParam(bundle.getParameter(9), plugin, 9);
	ImGui::PopID();
	ImGui::PushID(12);
	renderParam(bundle.getParameter(12), plugin, 12);
	ImGui::PopID();


	ImGui::Text("Detune LFO");

	renderLFO(plugin, ImVec2(ImGui::GetWindowContentRegionWidth(), 160), 11, 10, 14, 15, 16, 13, 17);

	ImGui::End();
}
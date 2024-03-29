#include "monosynthInstrumentPluginEditor.h"


monosynthInstrumentPluginEditor::monosynthInstrumentPluginEditor(PluginBase* plugin)
	: ImguiEffectEditor(plugin, 460, 680)
{
}


void monosynthInstrumentPluginEditor::imguiFrame()
{

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	const monosynthInstrument* glide = dynamic_cast<const monosynthInstrument*>(this->plugin.getEffect());
	if (glide)
	{
		renderADHSR(plugin, ImVec2(ImGui::GetWindowContentRegionWidth(), 300), &glide->getTempoData());
	}
	else
		renderADHSR(plugin, ImVec2(ImGui::GetWindowContentRegionWidth(), 300));
	

	renderParam( plugin, 8);

	renderParam( plugin, 9);

	renderParam( plugin, 12);


	ImGui::Separator();
	ImGui::Text("Detune LFO");
	monosynthInstrument* instr = dynamic_cast<monosynthInstrument*>(this->getEffectBase());

	renderLFO(plugin, instr->getLFO(), instr->getTempoData(), ImVec2(ImGui::GetWindowContentRegionWidth(), 260), 11, 10, 14, 15, 16, 13, 17);

	ImGui::End();
}
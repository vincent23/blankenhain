#include "PanPluginEditor.h"



PanPluginEditor::PanPluginEditor(PluginBase* plugin)
	: ImguiEffectEditor(plugin, 460, 400)
{
}


void PanPluginEditor::imguiFrame()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	renderParam( plugin, 0);

	renderParam( plugin, 1);
	renderParam(plugin, 9);
	
	PanEffect* effect =  dynamic_cast<PanEffect *>(this->getEffectBase());
	ImGui::Separator();
	ImGui::Text("Pan LFO");
	renderLFO(plugin, effect->getLFO(), effect->getTempoData(), ImVec2(ImGui::GetWindowContentRegionWidth(), 260) , 3, 2, 5, 6, 8, 4, 7);

	ImGui::End();
}
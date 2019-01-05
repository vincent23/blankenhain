#include "FilterPluginEditor.h"



FilterPluginEditor::FilterPluginEditor(PluginBase* plugin)
	: ImguiEffectEditor(plugin, 460, 400)
{
}


void FilterPluginEditor::imguiFrame()
{

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);


	renderParam( plugin, 0);
	renderParam( plugin, 1);
	renderParam( plugin, 2);
	renderParam( plugin, 3);
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::Text("Frequency Modulation LFO");

	FilterEffect* effect = dynamic_cast<FilterEffect*>(this->getEffectBase());
	renderLFO(plugin, effect->getLFO(), effect->getTempoData(), ImVec2(ImGui::GetWindowContentRegionWidth(), 260), 5,4,7,8,9,6,10);

	ImGui::End();
}
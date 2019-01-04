#include "ChorusPluginEditor.h"
#include "PluginParameterBundle.h"
#include "PluginBase.h"
#include "AuxFunc.h"
#include "ChorusPlugin.h"
#include "ChorusEffect.h"

#include <algorithm>
#include <string>
#include <imgui.h>

ChorusPluginEditor::ChorusPluginEditor(PluginBase* plugin)
	: ImguiEffectEditor(plugin, 500, 535)
{}


void ChorusPluginEditor::imguiFrame()
{

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::Begin("bh_chorus", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);


	ImGui::Text(std::string("Chorus: Use Delay Times from 8ms to 32ms.").c_str());
	ImGui::Text(std::string("Flanger: Use Delay Times from 1ms to 7ms with Feedback.").c_str());
	ImGui::Separator();
	ImGui::Separator();
	
	renderParam(plugin, 8);
	renderParam(plugin, 2);
	renderParam(plugin, 3);
	renderParam(plugin, 4);
	renderParam(plugin, 1);
	renderParam(plugin, 5, 1.f, 1u);

	ChorusEffect* effect = dynamic_cast<ChorusEffect *>(this->getEffectBase());
	ImGui::Separator();
	ImGui::Separator();

	ImGui::Text("LFO - Delay Line Sweep");

	renderLFO(plugin, effect->getLFO(), effect->getTempoData(), 
		ImVec2(ImGui::GetWindowContentRegionWidth(), 310),
		9, 0, 6, 10, 7, 11, -1);

	ImGui::End();
}
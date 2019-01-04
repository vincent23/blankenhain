#include "DistortionPluginEditor.h"



DistortionPluginEditor::DistortionPluginEditor(PluginBase* plugin)
	: ImguiEffectEditor(plugin, 570, 120)
{
}


void DistortionPluginEditor::imguiFrame()
{

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::Begin("bh_distortion", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);


	renderParam(plugin, 0);
	renderParam(plugin, 1);
	renderParam(plugin, 2);
	renderParam(plugin, 3);

	ImGui::End();
}
#include "ExamplePluginEditor.h"

#include <imgui.h>

ExamplePluginEditor::ExamplePluginEditor(PluginBase* plugin)
	: ImguiEffectEditor(plugin, 500, 500)
{}


void ExamplePluginEditor::imguiFrame()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	ImGui::End();
}
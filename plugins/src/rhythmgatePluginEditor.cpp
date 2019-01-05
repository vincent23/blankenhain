#include "RhythmgatePluginEditor.h"

#include "constants.h"
#include "NormalizedRange.h"
#include "PluginParameterBundle.h"
#include "ParameterBundle.h"
#include "AuxFunc.h"
#include "RhythmgateEffect.h"
#include "ImguiCustomFunctions.h"

#include <imgui.h>
#include <cmath>
#include <complex>

#include <windows.h>

RhythmgatePluginEditor::RhythmgatePluginEditor(PluginBase* plugin)
	: ImguiEffectEditor(plugin, 700, 520)
{}

void RhythmgatePluginEditor::imguiFrame()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	for (unsigned int i = 0u; i < 16u; i+=4)
	{
		renderParam(plugin, i);
		renderParam(plugin, i + 1);
		renderParam(plugin, i + 2);
		renderParam(plugin, i + 3);
		ImGui::Spacing();
	}
	ImGui::Separator();
	ImGui::Spacing();

	renderParam(plugin, 16);
	renderParam(plugin, 17);
	renderParam(plugin, 18);
	renderParam(plugin, 19);
	renderParam(plugin, 20);

	ImGui::End();
}
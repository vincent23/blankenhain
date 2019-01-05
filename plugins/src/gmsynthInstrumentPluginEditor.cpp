#include "gmsynthInstrumentPluginEditor.h"
#include "ImguiCustomFunctions.h"

#include "constants.h"
#include "NormalizedRange.h"
#include "PluginParameterBundle.h"
#include "ParameterBundle.h"
#include "AuxFunc.h"
#include "gmsynthInstrumentPlugin.h"
#include "gmInstrument.h"

#include <imgui.h>
#include <cmath>
#include <complex>

#include <windows.h>

gmsynthPluginEditor::gmsynthPluginEditor(PluginBase* plugin_)
	: ImguiEffectEditor(plugin_, 460, 460)
{
	gmInstrument** instruments = static_cast<gmsynthInstrumentPlugin*>(plugin_)->instruments;
	for (unsigned int i = 0; i < 235; i++) {
		instrumentNames.push_back(instruments[i]->name);
	}
}

void gmsynthPluginEditor::imguiFrame()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	renderADHSR(plugin, ImVec2(ImGui::GetWindowContentRegionWidth(), 300));

	renderParam(plugin, 8);

	DiscreteParameter const* instrumentParameter = static_cast<DiscreteParameter const*>(plugin.getParameters().getParameter(9));
	int selectedInstrument = static_cast<int>(instrumentParameter->getValueUnnormalized());
	if (ImGui::Combo("filter", &selectedInstrument, instrumentNames.data(), instrumentNames.size())) {
		plugin.setParameterAutomated(9, instrumentParameter->getRange().toNormalized(static_cast<float>(selectedInstrument) + .1f));
	}
	ImGui::End();
}
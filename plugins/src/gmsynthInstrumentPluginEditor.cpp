#include "gmsynthInstrumentPluginEditor.h"

#include "constants.h"
#include "NormalizedRange.h"
#include "PluginParameterBundle.h"
#include "ParameterBundle.h"
#include "AuxFunc.h"
#include "gmsynthInstrumentPlugin.h"

#include <imgui.h>
#include <cmath>
#include <complex>

#include <windows.h>

gmsynthPluginEditor::gmsynthPluginEditor(PluginBase* plugin)
	: ImguiEffectEditor(plugin, 200, 200)
{
}

void gmsynthPluginEditor::imguiFrame()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	char combo[235 * 4 + 1];
	combo[235 * 4] = 0;
	for (int i = 0; i < 235; i++) {
		combo[i * 4] = i / 100 + '0';
		combo[i * 4 + 1] = (i / 10) % 10 + '0';
		combo[i * 4 + 2] = i % 10 + '0';
		combo[i * 4 + 3] = 0;
	}
	static int item = 0;
	if (ImGui::Combo("filter", &item, combo)) {
		static_cast<gmsynthInstrumentPlugin*>(&plugin)->loadMidiInstrument(item);
	}
	ImGui::End();
}
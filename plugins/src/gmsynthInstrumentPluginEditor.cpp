#include "gmsynthInstrumentPluginEditor.h"

#include "constants.h"
#include "NormalizedRange.h"
#include "PluginParameterBundle.h"
#include "ParameterBundle.h"
#include "AuxFunc.h"
#include "gmsynthInstrument.h"

#include <imgui.h>
#include <cmath>
#include <complex>

#include <windows.h>

gmsynthPluginEditor::gmsynthPluginEditor(PluginBase* plugin)
	: ImguiEffectEditor(plugin, 700, 500)
{}

void gmsynthPluginEditor::imguiFrame()
{
	// some hacky gui code
	// don't use as an example

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	const unsigned int nPoints = 250;
	float points[nPoints];
	//double nyquistFrequency = constants::sampleRate * .5f;
	const PluginParameterBundle& bundle = plugin.getParameters();
	static int item = 0;
	ImGui::Combo("filter", &item, " 1st\0 2nd\0 3rd\0 4th\0 5th\0 6th\0 7th\0 8th\0\0");
	bool isOnPrevious = bundle.getParameterUnnormalized(item * 5 + 1) >= .5;
	bool isOn = isOnPrevious;
	if (ImGui::Checkbox("on", &isOn)) {
		OutputDebugStringA("test");
	}
	if (isOn != isOnPrevious) {
		// TODO
		plugin.setParameterAutomated(item * 5 + 1, isOn ? .75f : .25f);
	}
	NormalizedRange* frequencyRange = bundle.getConversions().getParameter(2);
	float unnormalizedFrequency = bundle.getParameterUnnormalized(item * 5 + 2);
	float frequencyMin = frequencyRange->getStart();
	float frequencyMax = frequencyRange->getEnd();
	float frequencySkew = frequencyRange->getSkew();
	if (ImGui::DragFloat("frequency", &unnormalizedFrequency, .01f, frequencyMin, frequencyMax, "%.3f", 1 / frequencySkew))
	{
		plugin.setParameterAutomated(item * 5 + 2, frequencyRange->toNormalized(unnormalizedFrequency));
	}
	NormalizedRange* gainRange = bundle.getConversions().getParameter(3);
	float unnormalizedGain = bundle.getParameterUnnormalized(item * 5 + 3);
	float gainMin = gainRange->getStart();
	float gainMax = gainRange->getEnd();
	float gainSkew = gainRange->getSkew();
	if (ImGui::DragFloat("Gain", &unnormalizedGain, .1f, gainMin, gainMax, "%.3f", 1 / gainSkew))
	{
		plugin.setParameterAutomated(item * 5 + 3, gainRange->toNormalized(unnormalizedGain));
	}
	NormalizedRange* qRange = bundle.getConversions().getParameter(4);
	float unnormalizedQ = bundle.getParameterUnnormalized(item * 5 + 4);
	float qMin = qRange->getStart();
	float qMax = qRange->getEnd();
	float qSkew = qRange->getSkew();
	if (ImGui::DragFloat("Q", &unnormalizedQ, .01f, qMin, qMax, "%.3f", 1 / qSkew))
	{
		plugin.setParameterAutomated(item * 5 + 4, qRange->toNormalized(unnormalizedQ));
	}
	int type = (int)bundle.getParameterUnnormalized(item * 5 + 5);
	if (ImGui::Combo("type", &type, "High pass\0Low shelf\0Bell\0Notch\0High shelf\0Low pass\0\0")) {
		plugin.setParameterAutomated(item * 5 + 5, ((type + .1f) / 6.));
	}
	for (unsigned int i = 0; i < nPoints; i++) {
		points[i] = 0;
	}
	ImGui::PlotLines("##freqresponse", points, nPoints, 0, 0, -15.f, 15.f, ImGui::GetContentRegionAvail());
	ImGui::End();
}
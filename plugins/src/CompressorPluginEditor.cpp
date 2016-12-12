#include "CompressorPluginEditor.h"
#include "PluginParameterBundle.h"
#include "PluginBase.h"
#include "AuxFunc.h"
#include "CompressorPlugin.h"
#include "CompressorEffect.h"

#include <algorithm>
#include <imgui.h>

CompressorPluginEditor::CompressorPluginEditor(PluginBase* plugin)
	: ImguiEffectEditor(plugin, 500, 500)
{}


void CompressorPluginEditor::imguiFrame()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	float envelopeDb = static_cast<CompressorPlugin*>(&plugin)->getEnvelope();
	ImGui::DragFloat("envDb", &envelopeDb);
	const PluginParameterBundle& bundle = plugin.getParameters();
	const unsigned int nPoints = 250;
	float points[nPoints];
	float threshold = bundle.getParameterUnnormalized(2);
	float ratio = bundle.getParameterUnnormalized(3);
	float knee = bundle.getParameterUnnormalized(4);
	for (unsigned int i = 0; i < nPoints; i++) {
		float dbIn = -42.f + 48.f * ((float)i / (nPoints - 1));
		points[i] = dbIn + CompressorEffect::compressorGain(threshold, ratio, knee, dbIn);
	}
	ImGui::PlotLines("##freqresponse", points, nPoints, 0, 0, -42.f, 6.f, ImGui::GetContentRegionAvail());
	ImGui::End();
}
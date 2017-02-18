#include "CompressorPluginEditor.h"
#include "PluginParameterBundle.h"
#include "PluginBase.h"
#include "AuxFunc.h"
#include "CompressorPlugin.h"
#include "CompressorEffect.h"

#include <algorithm>
#include <string>
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
	const PluginParameterBundle& bundle = plugin.getParameters();
	const unsigned int nPoints = 250;
	float points[nPoints];
	float threshold = bundle.getParameterUnnormalized(2);
	float ratio = bundle.getParameterUnnormalized(3);
	float knee = bundle.getParameterUnnormalized(4);
	const float dbMin = -42.f;
	const float dbMax = 6.f;
	for (unsigned int i = 0; i < nPoints; i++) {
		float dbIn = dbMin + (dbMax - dbMin) * ((float)i / (nPoints - 1));
		points[i] = dbIn + static_cast<float>(CompressorEffect::compressorGain(threshold, ratio, knee, dbIn));
	}

	// show gain reduction
	float envelopeDbIn = static_cast<CompressorPlugin*>(&plugin)->getEnvelope();
	float compressorGain = static_cast<float>(CompressorEffect::compressorGain(threshold, ratio, knee, envelopeDbIn));
	float gainReduction = compressorGain / dbMin;
	std::string overlay = std::to_string(compressorGain) + " dB";
	ImGui::ProgressBar(gainReduction, ImVec2(0, 0), overlay.c_str());
	ImGui::SameLine();
	ImGui::Text("Gain Reduction");

	// plot compressor curve
	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImVec2 plotSize = ImGui::GetContentRegionAvail();
	ImGui::PlotLines("##freqresponse", points, nPoints, 0, 0, dbMin, dbMax, plotSize);

	// show input level
	ImDrawList& drawList = *ImGui::GetWindowDrawList();
	float x = (envelopeDbIn - dbMin) / (dbMax - dbMin); // [0..1]
	float envelopeDbOut = envelopeDbIn + compressorGain;
	float y = 1.f - (envelopeDbOut - dbMin) / (dbMax - dbMin); // [0..1]
	drawList.AddCircleFilled(ImVec2(x * plotSize.x + pos.x, y * plotSize.y + pos.y), 5.f, ImColor(1.f, 1.f, 0.f));


	ImGui::End();
}
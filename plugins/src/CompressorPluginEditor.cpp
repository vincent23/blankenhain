#include "CompressorPluginEditor.h"
#include "PluginParameterBundle.h"
#include "PluginBase.h"
#include "AuxFunc.h"
#include "CompressorPlugin.h"

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
		points[i] = compressorMap(threshold, ratio, knee, dbIn);
	}
	ImGui::PlotLines("##freqresponse", points, nPoints, 0, 0, -42.f, 6.f, ImGui::GetContentRegionAvail());
	ImGui::End();
}

float CompressorPluginEditor::compressorMap(float threshold, float ratio, float knee, float dbIn) const {
	// probably this can be done in a smarter (branchless) way
	float kneeStart = (threshold - knee);
	float kneeEnd = threshold + knee;
	if (dbIn < kneeStart) {
		return dbIn;
	}
	else if (dbIn < kneeEnd) {
		// quadratic bezier for knee
		//float t = (dbIn - kneeStart) / kneeWidth;
		//float a = kneeStart;
		//float b = threshold;
		//float c = threshold + knee / ratio;
		//float ab = a * (1.f - t) + b * t;
		//float bc = b * (1.f - t) + c * t;
		//dbOut = ab * (1.f - t) + bc * t;
		// simplified version of the above:
		float t = (dbIn - kneeStart) / knee * .5f;
		return threshold + knee * (t * t * (1. / ratio - 1.f) + 2.f * t - 1.f);
	}
	else {
		return threshold + (dbIn - threshold) / ratio;
	}
}
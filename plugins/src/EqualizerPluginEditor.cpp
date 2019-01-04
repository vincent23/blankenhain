#include "EqualizerPluginEditor.h"

#include "constants.h"
#include "NormalizedRange.h"
#include "PluginParameterBundle.h"
#include "ParameterBundle.h"
#include "AuxFunc.h"
#include "EqualizerEffect.h"
#include "ImguiCustomFunctions.h"

#include <imgui.h>
#include <cmath>
#include <complex>

#include <windows.h>

EqualizerPluginEditor::EqualizerPluginEditor(PluginBase* plugin)
	: ImguiEffectEditor(plugin, 700, 500)
{}

void EqualizerPluginEditor::imguiFrame()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	ImGui::RadioButton("1st", &item, 0); ImGui::SameLine();
	ImGui::RadioButton("2nd", &item, 1); ImGui::SameLine();
	ImGui::RadioButton("3rd", &item, 2); ImGui::SameLine();
	ImGui::RadioButton("4th", &item, 3); ImGui::SameLine();
	ImGui::RadioButton("5th", &item, 4); ImGui::SameLine();
	ImGui::RadioButton("6th", &item, 5); ImGui::SameLine();
	ImGui::RadioButton("7th", &item, 6); ImGui::SameLine();
	ImGui::RadioButton("8th", &item, 7);


	ImGui::Separator();
	renderParam(plugin, item * 5 + 1); // on
	renderParam(plugin, item * 5 + 2, 0.0007f); // freq
	renderParam(plugin, item * 5 + 3); // gain
	renderParam(plugin, item * 5 + 4); // q
	renderParam(plugin, item * 5 + 5); // filter style

	// plot equalizer response
	const unsigned int nPoints = 250;
	float points[nPoints];
	double nyquist = constants::sampleRate * .5;
	for (unsigned int i = 0; i < nPoints; i++) {
		points[i] = 0;
	}
	const PluginParameterBundle& bundle = plugin.getParameters();
	const NormalizedRange& frequencyRange = bundle.getParameter(2)->getRange();
	for (unsigned int filterIndex = 0; filterIndex < numberOfEqualizerFilters; filterIndex++) {
		bool off = bundle.getParameterUnnormalized(filterIndex * 5 + 1) < .5;
		if (off) {
			continue;
		}
		double gain = bundle.getParameterUnnormalized(filterIndex * 5 + 3);
		double cutoff = bundle.getParameterUnnormalized(filterIndex * 5 + 2);
		double g = std::tan(constants::pi / constants::sampleRate * cutoff);
		double g2 = g * g;
		double k = 1 / bundle.getParameterUnnormalized(filterIndex * 5 + 4);
		double gk = g * k;
		int type = (int)bundle.getParameterUnnormalized(filterIndex * 5 + 5);
		for (unsigned int i = 0; i < nPoints; i++) {
			double frequency = frequencyRange.fromNormalized((float)i / (nPoints - 1));
			// TODO move angular frequency conversion to a common place
			double omega = frequency * constants::pi / nyquist;
			//double omega = ((double)i / (nPoints - 1)) * constants::pi;
			auto z = std::exp(std::complex<double>(0, omega));
			auto onePlusZ2 = (double)1 + z;
			onePlusZ2 *= onePlusZ2;
			auto zMinusOne2 = z - (double)1;
			zMinusOne2 *= zMinusOne2;
			auto z2MinusOne = z * z - (double)1;
			std::complex<double> H(1);
			double m0, m1, m2;
			double A = std::pow(10., gain / 40.);
			double sqrtA = std::sqrt(A);
			switch (type) {
			case 0: // high pass
				H = zMinusOne2;
				H /= zMinusOne2 + g2 * onePlusZ2 + gk * z2MinusOne;
				break;
			case 1: // low shelf
				m1 = (A - 1) * k;
				m2 = A * A - 1;
				H = A * zMinusOne2 + g2 * (1. + m2) * onePlusZ2 + sqrtA * g * (k + m1) * z2MinusOne;
				H /= A * zMinusOne2 + g2 * onePlusZ2 + sqrtA * gk * z2MinusOne;
				break;
			case 2: // bell
				m1 = k * (A * A - 1) / A;
				H = gk * z2MinusOne + A * (g * ((double)1 + z) * (m1 * (z - (double)1)) + (zMinusOne2 + g2 * onePlusZ2));
				H /= gk * z2MinusOne + A * (zMinusOne2 + g2 * onePlusZ2);
				break;
			case 3: // notch
				H = zMinusOne2 + g2 * onePlusZ2;
				H /= zMinusOne2 + g2 * onePlusZ2 + gk * z2MinusOne;
				break;
			case 4: // high shelf
				m0 = A * A;
				m1 = -(A - 1) * A * k;
				m2 = 1 - A * A;
				H = sqrtA * g * ((double)1 + z) * (m1 * (z - (double)1) + sqrtA * g * m2 * ((double)1 + z));
				H += m0 * (zMinusOne2 + A * g2 * onePlusZ2 + sqrtA * gk * z2MinusOne);
				H /= zMinusOne2 + A * g2 * onePlusZ2 + sqrtA * gk * z2MinusOne;
				break;
			case 5: // low pass
				H = g2 * onePlusZ2;
				H /= zMinusOne2 + g2 * onePlusZ2 + gk * z2MinusOne;
				break;
			}
			points[i] += aux::linearToDecibel((float)std::abs(H));
		}
	}
	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImVec2 plotSize = ImGui::GetContentRegionAvail();
	ImDrawList& drawList = *ImGui::GetWindowDrawList();
	float const y0 = .5f;
	float const y_p6 = .5f + (6.f / 15.f) * .5f;
	float const y_m6 = .5f - (6.f / 15.f) * .5f;
	ImU32 lightgrey = ImColor(150, 150, 150);
	drawList.AddLine(ImVec2(pos.x, y0 * plotSize.y + pos.y), ImVec2(plotSize.x + pos.x, y0 * plotSize.y + pos.y), lightgrey);
	drawList.AddLine(ImVec2(pos.x, y_p6 * plotSize.y + pos.y), ImVec2(plotSize.x + pos.x, y_p6 * plotSize.y + pos.y), lightgrey);
	drawList.AddLine(ImVec2(pos.x, y_m6 * plotSize.y + pos.y), ImVec2(plotSize.x + pos.x, y_m6 * plotSize.y + pos.y), lightgrey);

	ImGui::PlotLines("##freqresponse", points, nPoints, 0, 0, -15.f, 15.f, plotSize);

	ImGui::End();
}
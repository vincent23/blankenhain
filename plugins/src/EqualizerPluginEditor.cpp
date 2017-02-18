#include "EqualizerPluginEditor.h"

#include "constants.h"
#include "NormalizedRange.h"
#include "PluginParameterBundle.h"
#include "ParameterBundle.h"
#include "AuxFunc.h"
#include "EqualizerEffect.h"

#include <imgui.h>
#include <cmath>
#include <complex>

#include <windows.h>

EqualizerPluginEditor::EqualizerPluginEditor(PluginBase* plugin)
	: ImguiEffectEditor(plugin, 700, 500)
{}

void EqualizerPluginEditor::imguiFrame()
{
	// some hacky gui code
	// don't use as an example

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	const unsigned int nPoints = 250;
	float points[nPoints];
	double nyquist = constants::sampleRate * .5;
	const PluginParameterBundle& bundle = plugin.getParameters();
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
	NormalizedRange const* frequencyRange = bundle.getParameter(2);
	float unnormalizedFrequency = bundle.getParameterUnnormalized(item * 5 + 2);
	float frequencyMin = frequencyRange->getStart();
	float frequencyMax = frequencyRange->getEnd();
	float frequencySkew = frequencyRange->getSkew();
	if (ImGui::DragFloat("frequency", &unnormalizedFrequency, .01f, frequencyMin, frequencyMax, "%.3f", 1 / frequencySkew))
	{
		plugin.setParameterAutomated(item * 5 + 2, frequencyRange->toNormalized(unnormalizedFrequency));
	}
	NormalizedRange const* gainRange = bundle.getParameter(3);
	float unnormalizedGain = bundle.getParameterUnnormalized(item * 5 + 3);
	float gainMin = gainRange->getStart();
	float gainMax = gainRange->getEnd();
	float gainSkew = gainRange->getSkew();
	if (ImGui::DragFloat("Gain", &unnormalizedGain, .1f, gainMin, gainMax, "%.3f", 1 / gainSkew))
	{
		plugin.setParameterAutomated(item * 5 + 3, gainRange->toNormalized(unnormalizedGain));
	}
	NormalizedRange const* qRange = bundle.getParameter(4);
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
		plugin.setParameterAutomated(item * 5 + 5, ((type + .1f) / 6.f));
	}
	for (unsigned int i = 0; i < nPoints; i++) {
		points[i] = 0;
	}
	for (unsigned int filterIndex = 0; filterIndex < EqualizerEffect::numberOfFilters; filterIndex++) {
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
			double frequency = frequencyRange->fromNormalized((float)i / (nPoints - 1));
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
	ImGui::PlotLines("##freqresponse", points, nPoints, 0, 0, -15.f, 15.f, ImGui::GetContentRegionAvail());
	ImGui::End();
}
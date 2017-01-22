#include "glidePolyblepInstrumentPluginEditor.h"



glidePolyblepInstrumentPluginEditor::glidePolyblepInstrumentPluginEditor(PluginBase* plugin)
	: ImguiEffectEditor(plugin, 400, 680)
{
}


void glidePolyblepInstrumentPluginEditor::imguiFrame()
{

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	const PluginParameterBundle& bundle = plugin.getParameters();



	renderADHSR(plugin, ImVec2(ImGui::GetWindowContentRegionWidth(), 300));
	
	int waveTypeCarrier = bundle.getParameterNormalized(8) * 4;
	ImGui::RadioButton("Sine", &waveTypeCarrier, 1);
	ImGui::SameLine();
	ImGui::RadioButton("Sawtooth", &waveTypeCarrier, 2);
	ImGui::SameLine();
	ImGui::RadioButton("Square", &waveTypeCarrier, 3);
	ImGui::SameLine();
	ImGui::RadioButton("Triangle", &waveTypeCarrier, 4);
	if (waveTypeCarrier != bundle.getParameterNormalized(8) * 4)
	  plugin.setParameterAutomated(8, (waveTypeCarrier / 4.f));
	
	NormalizedRange const* range = nullptr;
	float min = 0.f;
	float max = 0.f;
	float skew = 0.f;


	// glide
	range = bundle.getParameter(9);
	min = range->getStart();
	max = range->getEnd();
	skew = range->getSkew();
	
	float value = bundle.getParameterUnnormalized(9);
	if (ImGui::DragFloat((bundle.getParameterName(9)).c_str(), &value, 0.0001f, min, max, "%.5f", 1 / skew))
	  plugin.setParameterAutomated(9, range->toNormalized(value));
	ImGui::SameLine();
	if (ImGui::Button("ResetGlide"))
	{
		plugin.setParameterAutomated(9, bundle.getParameter(9)->getDefaultValueNormalized());
	}

	// detune
	range = bundle.getParameter(12);
	min = range->getStart();
	max = range->getEnd();
	skew = range->getSkew();

	value = bundle.getParameterUnnormalized(12);
	if (ImGui::DragFloat((bundle.getParameterName(12)).c_str(), &value, 0.01f, min, max, "%.3f", 1 / skew))
		plugin.setParameterAutomated(12, range->toNormalized(value));
	ImGui::SameLine();

	if (ImGui::Button("ResetDetune"))
	{
		plugin.setParameterAutomated(12, bundle.getParameter(12)->getDefaultValueNormalized());
	}

	ImGui::Text("Detune LFO");

	renderLFO(plugin, ImVec2(ImGui::GetWindowContentRegionWidth(), 160), 11, 10, 14, 15, 16, 13, 17);

	ImGui::End();
}
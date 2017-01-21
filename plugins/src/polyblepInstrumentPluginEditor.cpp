#include "polyblepInstrumentPluginEditor.h"



polyblepInstrumentPluginEditor::polyblepInstrumentPluginEditor(PluginBase* plugin)
	: ImguiEffectEditor(plugin, 800, 805)
{
}


void polyblepInstrumentPluginEditor::imguiFrame()
{

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	const PluginParameterBundle& bundle = plugin.getParameters();



	renderADHSR(plugin, ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 300));
	
	ImGui::BeginChild("Waveform", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.48f, 300), true);
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
	ImGui::EndChild();
	
	NormalizedRange const* range = nullptr;
	float min = 0.f;
	float max = 0.f;
	float skew = 0.f;


	// detune
	range = bundle.getParameter(11);
	min = range->getStart();
	max = range->getEnd();
	skew = range->getSkew();
	
	float value = bundle.getParameterUnnormalized(11);
	if (ImGui::DragFloat((bundle.getParameterName(11)).c_str(), &value, 0.01f, min, max, "%.3f", 1 / skew))
	  plugin.setParameterAutomated(11, range->toNormalized(value));
	if (ImGui::Button("Reset"))
	{
		plugin.setParameterAutomated(11, bundle.getParameter(11)->getDefaultValueNormalized());
	}
	
	//    range = bundle.getParameterBundle().getParameter(currentParamInt + 1);
	//    min = range->getStart();
	//    max = range->getEnd();
	//    skew = range->getSkew();
	//
	//    int modType = bundle.getParameterUnnormalized(currentParamInt + 1) * 3;
	//    ImGui::RadioButton("FM", &modType, 0); ImGui::SameLine();
	//    ImGui::RadioButton("PM", &modType, 1); ImGui::SameLine();
	//    ImGui::RadioButton("AM", &modType, 2);
	//    int previousModType = static_cast<int>(bundle.getParameterUnnormalized(currentParamInt + 1) * 3);
	//    if (modType != previousModType)
	//    {
	//      plugin.setParameterAutomated(currentParamInt + 1, range->toNormalized(modType / 3.f));
	//    }
	//
	//
	//    range = bundle.getParameterBundle().getParameter(currentParamInt + 2);
	//    min = range->getStart();
	//    max = range->getEnd();
	//    skew = range->getSkew();
	//
	//    value = bundle.getParameterUnnormalized(currentParamInt + 2);
	//    if (ImGui::DragFloat(("osc" + std::to_string(i + 1) + " " + bundle.getParameterName(currentParamInt + 2)).c_str(), &value, 0.01f, min, max, "%.3f", 1 / skew))
	//      plugin.setParameterAutomated(currentParamInt + 2, range->toNormalized(value));
	//
	//
	//    range = bundle.getParameterBundle().getParameter(currentParamInt + 3);
	//    min = range->getStart();
	//    max = range->getEnd();
	//    skew = range->getSkew();
	//
	//    int targetOsc = bundle.getParameterUnnormalized(currentParamInt + 3);
	//    ImGui::SliderInt(("osc" + std::to_string(i + 1) + " target").c_str(), &targetOsc, i + 1, max);
	//    if (targetOsc != bundle.getParameterUnnormalized(currentParamInt + 3))
	//      plugin.setParameterAutomated(currentParamInt + 3, range->toNormalized(targetOsc * 1.f));
	//
	//    range = bundle.getParameterBundle().getParameter(currentParamInt + 6);
	//    min = range->getStart();
	//    max = range->getEnd();
	//    skew = range->getSkew();
	//
	//    int waveType = bundle.getParameterUnnormalized(currentParamInt + 6) * 4;
	//    ImGui::RadioButton("Sine", &waveType, 0);
	//    ImGui::SameLine();
	//    ImGui::RadioButton("Sawtooth", &waveType, 1);
	//    ImGui::SameLine();
	//    ImGui::RadioButton("Square", &waveType, 2);
	//    ImGui::SameLine();
	//    ImGui::RadioButton("Triangle", &waveType, 3);
	//    if (waveType != bundle.getParameterUnnormalized(currentParamInt + 6) * 4)
	//      plugin.setParameterAutomated(currentParamInt + 6, range->toNormalized(waveType / 4.f));
	//    ImGui::Separator();
	
	
	// missing pop -> causes crash
	// ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
	
	
	
	ImGui::End();
}
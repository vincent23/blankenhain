#include "fmInstrumentPluginEditor.h"



FmInstrumentPluginEditor::FmInstrumentPluginEditor(PluginBase* plugin)
	: ImguiEffectEditor(plugin, 800, 805)
{
}


void FmInstrumentPluginEditor::imguiFrame()
{

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	const PluginParameterBundle& bundle = plugin.getParameters();

	NormalizedRange const* range = nullptr;
	float min = 0.f;
	float max = 0.f;
	float skew = 0.f;

	// Top section
	// Left: AHDSR
	// Right: Carrier Waveform and carrier stuff

	renderADHSR(plugin, ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 300));
	ImGui::SameLine();
	
	ImGui::BeginChild("Carrier", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.48f, 300), true);
	int waveTypeCarrier = bundle.getParameterUnnormalized(98) * 4;
	ImGui::RadioButton("Sine", &waveTypeCarrier, 0);
	ImGui::SameLine();
	ImGui::RadioButton("Sawtooth", &waveTypeCarrier, 1);
	ImGui::SameLine();
	ImGui::RadioButton("Square", &waveTypeCarrier, 2);
	ImGui::SameLine();
	ImGui::RadioButton("Triangle", &waveTypeCarrier, 3);
	if (waveTypeCarrier != bundle.getParameterUnnormalized(98) * 4)
	  plugin.setParameterAutomated(98, (waveTypeCarrier / 4.f));
	ImGui::EndChild();
	
	
	// Bottom section
	// Left & Right: Modulation Oscillators

	ImGui::BeginChild("Modulation1", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 480), false);
	
	for (unsigned int i = 0u; i < 4u; i++)
	{
	
	  ImGui::PushID(i);
	  unsigned int currentParamInt = 8 + i * 11;
	
	  float isOn = bundle.getParameterUnnormalized(currentParamInt + 7);
	  if (ImGui::Button("On/Off"))
	    plugin.setParameterAutomated(currentParamInt + 7, isOn == 1.f ? 0.f : 1.f);
	
	  ImGui::SameLine;
	// On / Off
	  if (isOn == 1.f)
	  {
	    int isLFO = static_cast<float>(bundle.getParameterUnnormalized(currentParamInt + 8));
	    ImGui::RadioButton("LFO off", &isLFO, 0);
	    ImGui::SameLine();
	    ImGui::RadioButton("LFO on", &isLFO, 1);
	    if (isLFO != static_cast<float>(bundle.getParameterUnnormalized(currentParamInt + 8)))
	      plugin.setParameterAutomated(currentParamInt + 8, isLFO == 1 ? 1.f : 0.f);
	
	    ImGui::SameLine;
	    float tempoSyncOn = bundle.getParameterUnnormalized(currentParamInt + 9);
	    if (ImGui::Button("TempoSync"))
	      plugin.setParameterAutomated(currentParamInt + 9, tempoSyncOn == 1.f ? 0.f : 1.f);
	
	    if (tempoSyncOn == 1.f)
	    {
	      range = bundle.getParameter(currentParamInt + 10);
	      min = range->getStart();
	      max = range->getEnd();
	      skew = range->getSkew();
	
	      float value = bundle.getParameterUnnormalized(currentParamInt + 10);
	      if (ImGui::DragFloat(("temposync" + std::to_string(i + 1) + " " + bundle.getParameterName(currentParamInt)).c_str(), &value, 0.01f, min, max, "%.3f", 1 / skew))
	        plugin.setParameterAutomated(currentParamInt + 10, range->toNormalized(value));
	
	    }
	    // freq
	    range = bundle.getParameter(currentParamInt);
	    min = range->getStart();
	    max = range->getEnd();
	    skew = range->getSkew();
	
	    float value = bundle.getParameterUnnormalized(currentParamInt);
	    if (ImGui::DragFloat(("osc" + std::to_string(i + 1) + " " + bundle.getParameterName(currentParamInt)).c_str(), &value, 0.01f, min, max, "%.3f", 1 / skew))
	      plugin.setParameterAutomated(currentParamInt, range->toNormalized(value));
	
	    range = bundle.getParameter(currentParamInt + 1);
	    min = range->getStart();
	    max = range->getEnd();
	    skew = range->getSkew();
	
	    int modType = bundle.getParameterUnnormalized(currentParamInt + 1) * 3;
	    ImGui::RadioButton("FM", &modType, 0); ImGui::SameLine();
	    ImGui::RadioButton("PM", &modType, 1); ImGui::SameLine();
	    ImGui::RadioButton("AM", &modType, 2);
	    int previousModType = static_cast<int>(bundle.getParameterUnnormalized(currentParamInt + 1) * 3);
	    if (modType != previousModType)
	    {
	      plugin.setParameterAutomated(currentParamInt + 1, range->toNormalized(modType / 3.f));
	    }
	
	
	    range = bundle.getParameter(currentParamInt + 2);
	    min = range->getStart();
	    max = range->getEnd();
	    skew = range->getSkew();
	
	    value = bundle.getParameterUnnormalized(currentParamInt + 2);
	    if (ImGui::DragFloat(("osc" + std::to_string(i + 1) + " " + bundle.getParameterName(currentParamInt + 2)).c_str(), &value, 0.01f, min, max, "%.3f", 1 / skew))
	      plugin.setParameterAutomated(currentParamInt + 2, range->toNormalized(value));
	
	
	    range = bundle.getParameter(currentParamInt + 3);
	    min = range->getStart();
	    max = range->getEnd();
	    skew = range->getSkew();
	
	    int targetOsc = bundle.getParameterUnnormalized(currentParamInt + 3);
	    ImGui::SliderInt(("osc" + std::to_string(i + 1) + " target").c_str(), &targetOsc, i + 1, max);
	    if (targetOsc != bundle.getParameterUnnormalized(currentParamInt + 3))
	      plugin.setParameterAutomated(currentParamInt + 3, range->toNormalized(targetOsc * 1.f));
	
	    range = bundle.getParameter(currentParamInt + 6);
	    min = range->getStart();
	    max = range->getEnd();
	    skew = range->getSkew();
	
	    int waveType = bundle.getParameterUnnormalized(currentParamInt + 6) * 4;
	    ImGui::RadioButton("Sine", &waveType, 0);
	    ImGui::SameLine();
	    ImGui::RadioButton("Sawtooth", &waveType, 1);
	    ImGui::SameLine();
	    ImGui::RadioButton("Square", &waveType, 2);
	    ImGui::SameLine();
	    ImGui::RadioButton("Triangle", &waveType, 3);
	    if (waveType != bundle.getParameterUnnormalized(currentParamInt + 6) * 4)
	      plugin.setParameterAutomated(currentParamInt + 6, range->toNormalized(waveType / 4.f));
	    ImGui::Separator();
	
	  }
	  ImGui::PopID();
	}
	
	ImGui::EndChild();
	ImGui::SameLine();
	
	ImGui::BeginChild("Modulation2", ImVec2(0, 480), false);
	for (unsigned int i = 4u; i < 8u; i++)
	{
	
	  ImGui::PushID(i);
	  unsigned int currentParamInt = 8 + i * 11;
	
	  float isOn = bundle.getParameterUnnormalized(currentParamInt + 7);
	  if (ImGui::Button("On/Off"))
	    plugin.setParameterAutomated(currentParamInt + 7, isOn == 1.f ? 0.f : 1.f);
	
	  ImGui::SameLine;
	
	  if (isOn == 1.f)
	  {
	    int isLFO = static_cast<float>(bundle.getParameterUnnormalized(currentParamInt + 8));
	    ImGui::RadioButton("LFO off", &isLFO, 0);
	    ImGui::SameLine();
	    ImGui::RadioButton("LFO on", &isLFO, 1);
	    if (isLFO != static_cast<float>(bundle.getParameterUnnormalized(currentParamInt + 8)))
	      plugin.setParameterAutomated(currentParamInt + 8, isLFO == 1 ? 1.f : 0.f);
	
	    ImGui::SameLine;
	    float tempoSyncOn = bundle.getParameterUnnormalized(currentParamInt + 9);
	    if (ImGui::Button("TempoSync"))
	      plugin.setParameterAutomated(currentParamInt + 9, tempoSyncOn == 1.f ? 0.f : 1.f);
	
	    if (tempoSyncOn == 1.f)
	    {
	      range = bundle.getParameter(currentParamInt + 10);
	      min = range->getStart();
	      max = range->getEnd();
	      skew = range->getSkew();
	
	      float value = bundle.getParameterUnnormalized(currentParamInt + 10);
	      if (ImGui::DragFloat(("temposync" + std::to_string(i + 1) + " " + bundle.getParameterName(currentParamInt)).c_str(), &value, 0.01f, min, max, "%.3f", 1 / skew))
	        plugin.setParameterAutomated(currentParamInt + 10, range->toNormalized(value));
	
	    }
	    // freq
	    range = bundle.getParameter(currentParamInt);
	    min = range->getStart();
	    max = range->getEnd();
	    skew = range->getSkew();
	
	    float value = bundle.getParameterUnnormalized(currentParamInt);
	    if (ImGui::DragFloat(("osc" + std::to_string(i + 1) + " " + bundle.getParameterName(currentParamInt)).c_str(), &value, 0.01f, min, max, "%.3f", 1 / skew))
	      plugin.setParameterAutomated(currentParamInt, range->toNormalized(value));
	
	    range = bundle.getParameter(currentParamInt + 1);
	    min = range->getStart();
	    max = range->getEnd();
	    skew = range->getSkew();
	
	    int modType = bundle.getParameterUnnormalized(currentParamInt + 1) * 3;
	    ImGui::RadioButton("FM", &modType, 0); ImGui::SameLine();
	    ImGui::RadioButton("PM", &modType, 1); ImGui::SameLine();
	    ImGui::RadioButton("AM", &modType, 2);
	    int previousModType = static_cast<int>(bundle.getParameterUnnormalized(currentParamInt + 1) * 3);
	    if (modType != previousModType)
	    {
	      plugin.setParameterAutomated(currentParamInt + 1, range->toNormalized(modType / 3.f));
	    }
	
	
	    range = bundle.getParameter(currentParamInt + 2);
	    min = range->getStart();
	    max = range->getEnd();
	    skew = range->getSkew();
	
	    value = bundle.getParameterUnnormalized(currentParamInt + 2);
	    if (ImGui::DragFloat(("osc" + std::to_string(i + 1) + " " + bundle.getParameterName(currentParamInt + 2)).c_str(), &value, 0.01f, min, max, "%.3f", 1 / skew))
	      plugin.setParameterAutomated(currentParamInt + 2, range->toNormalized(value));
	
	
	    range = bundle.getParameter(currentParamInt + 3);
	    min = range->getStart();
	    max = range->getEnd();
	    skew = range->getSkew();
	
	    int targetOsc = bundle.getParameterUnnormalized(currentParamInt + 3);
	    ImGui::SliderInt(("osc" + std::to_string(i + 1) + " target").c_str(), &targetOsc, i + 1, max);
	    if (targetOsc != bundle.getParameterUnnormalized(i * 7 + 8 + 3))
	      plugin.setParameterAutomated(currentParamInt + 3, range->toNormalized(targetOsc * 1.f));
	
	    range = bundle.getParameter(currentParamInt + 6);
	    min = range->getStart();
	    max = range->getEnd();
	    skew = range->getSkew();
	
	    int waveType = bundle.getParameterUnnormalized(currentParamInt + 6) * 4;
	    ImGui::RadioButton("Sine", &waveType, 0);
	    ImGui::SameLine();
	    ImGui::RadioButton("Sawtooth", &waveType, 1);
	    ImGui::SameLine();
	    ImGui::RadioButton("Square", &waveType, 2);
	    ImGui::SameLine();
	    ImGui::RadioButton("Triangle", &waveType, 3);
	    if (waveType != bundle.getParameterUnnormalized(currentParamInt + 6) * 4)
	      plugin.setParameterAutomated(currentParamInt + 6, range->toNormalized(waveType / 4.f));
	    ImGui::Separator();
	  }
	  ImGui::PopID();
	
	}
	
	ImGui::EndChild();
	
	
	// missing pop -> causes crash
	// ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
	
	
	
	ImGui::End();
}
#pragma once
#include <imgui.h>
#include "FloatParameter.h"
#include "PluginBase.h"
#include "VoiceState.h"
#include "InstrumentBase.h"

#ifndef IM_ARRAYSIZE(_ARR)
#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))
#endif
static void renderParam(FloatParameter const* param, PluginBase& plugin, unsigned int paramIndex)
{

	if (dynamic_cast<BoolParameter const*>(param))
	{
		BoolParameter const* cParam = dynamic_cast<BoolParameter const*>(param);
		const bool before = cParam->getValue();
		int after = cParam->getValue();

		ImGui::Text(cParam->getName().c_str()); ImGui::SameLine();
		ImGui::RadioButton("Off", &after, 0); ImGui::SameLine();
		ImGui::RadioButton("On", &after, 1);
		plugin.setParameterAutomated(paramIndex, static_cast<float>(after));

	}
	else if (dynamic_cast<OptionParameter const*>(param))
	{
		OptionParameter const* cParam = dynamic_cast<OptionParameter const*>(param);
		const int before(cParam->getCurrentNumber());
		int current(cParam->getCurrentNumber());
		for (unsigned int i = 0u; i < cParam->getNumberOfPossibleValues(); i++)
		{
			ImGui::RadioButton(cParam->getOptionName(i).c_str(), &current, i);
			if (i != cParam->getNumberOfPossibleValues() - 1u)
				ImGui::SameLine();
		}
		if (before != current)
		{
			float normalized = static_cast<float>(current) / static_cast<float>(cParam->getNumberOfPossibleValues());
			plugin.setParameterAutomated(paramIndex, normalized);
		}
	}
	else if (dynamic_cast<DiscreteParameter const*>(param))
	{

		DiscreteParameter const* cParam = dynamic_cast<DiscreteParameter const*>(param);
		const int before(cParam->getCurrentNumber());
		int current(cParam->getCurrentNumber());

		//const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK" };
		const char** items = new const char*[cParam->getNumberOfPossibleValues()];
		BhString* comboBoxValue = new BhString[cParam->getNumberOfPossibleValues()];
		for (unsigned int i = 0u; i < cParam->getNumberOfPossibleValues(); i++)
		{
			comboBoxValue[i] = std::to_string(*(cParam->getPossibleValue(i)));
			items[i] = comboBoxValue[i].c_str();
		}
		ImGui::Combo(cParam->getName().c_str(), &current, items, cParam->getNumberOfPossibleValues());
		if (before != current)
		{
			float normalized = static_cast<float>(current) / static_cast<float>(cParam->getNumberOfPossibleValues());
			plugin.setParameterAutomated(paramIndex, normalized);
		}
		delete[] items;
		delete[] comboBoxValue;
	}
	else
	{
		const NormalizedRange* range = nullptr;
		float min = 0.f;
		float max = 0.f;
		float skew = 0.f;

		range = param;
		min = range->getStart();
		max = range->getEnd();
		skew = range->getSkew();
		float unnormalized = param->getValueUnnormalized();
		if (ImGui::DragFloat(param->getName().c_str(), &unnormalized, 0.001, min * 1.002, max * 0.998, "%.3f", 1 / skew))
			plugin.setParameterAutomated(paramIndex, range->toNormalized(unnormalized));
		ImGui::SameLine();
		if (ImGui::Button("Reset"))
		{
			plugin.setParameterAutomated(paramIndex, param->getDefaultValueNormalized());
		}
	
	}
}

static void renderADHSR(PluginBase& plugin, ImVec2 size = ImGui::GetContentRegionAvail(), unsigned int paramAttack = 0u, unsigned int paramHold = 1u, unsigned int paramHoldlevel = 2u,
	unsigned int paramDecay = 3u, unsigned int paramSustainbool = 4u, unsigned int paramSustain = 5u
	, unsigned int paramSustainlevel = 6u, unsigned int paramRelease = 7u)
{
	ImGui::BeginChild("adsrsub", size, true);
	const unsigned int nPoints = 250;
	float points[250];

	PluginParameterBundle const& bundle = plugin.getParameters();

	ImGui::PushID(paramAttack);
	renderParam(bundle.getParameter(paramAttack), plugin, paramAttack);
	ImGui::PopID();

	ImGui::PushID(paramHold);
	renderParam(bundle.getParameter(paramHold), plugin, paramHold);
	ImGui::PopID();

	ImGui::PushID(paramHoldlevel);
	renderParam(bundle.getParameter(paramHoldlevel), plugin, paramHoldlevel);
	ImGui::PopID();

	ImGui::PushID(paramDecay);
	renderParam(bundle.getParameter(paramDecay), plugin, paramDecay);
	ImGui::PopID();

	ImGui::PushID(paramSustainbool);
	renderParam(bundle.getParameter(paramSustainbool), plugin, paramSustainbool);
	ImGui::PopID();

	ImGui::PushID(paramSustain);
	renderParam(bundle.getParameter(paramSustain), plugin, paramSustain);
	ImGui::PopID();

	ImGui::PushID(paramSustainlevel);
	renderParam(bundle.getParameter(paramSustainlevel), plugin, paramSustainlevel);
	ImGui::PopID();

	ImGui::PushID(paramRelease);
	renderParam(bundle.getParameter(paramRelease), plugin, paramRelease);
	ImGui::PopID();

	// Plot adhsr
	float length = bundle.getParameter(paramAttack)->getValueUnnormalized() 
		+ bundle.getParameter(paramHold)->getValueUnnormalized()
		+ bundle.getParameter(paramDecay)->getValueUnnormalized()
		+ bundle.getParameter(paramSustain)->getValueUnnormalized()
		+ bundle.getParameter(paramRelease)->getValueUnnormalized();

	std::string lengthStr = "Length of visualized curve: " + std::to_string(length) + " ms.";
	ImGui::Text(lengthStr.c_str());


	if (length < 1000.f)
		length = 1000.f;
	else if (length > 5000.f)
		length = 5000.f;
	float incrementForVisualization = length / static_cast<float>(nPoints);
	VoiceState dummy;
	dummy.on(0u, 69u, 100u);
	for (unsigned int i = 0; i < nPoints; i++)
	{
		points[i] = 1.f;
		performAHDSR<float>(points, dummy, aux::millisecToSamples(i * incrementForVisualization), i,
			bundle.getParameter(paramAttack)->getValueUnnormalized(), 
			bundle.getParameter(paramRelease)->getValueUnnormalized(), 
			bundle.getParameter(paramHold)->getValueUnnormalized(), 
			bundle.getParameter(paramDecay)->getValueUnnormalized(), 
			bundle.getParameter(paramSustain)->getValueUnnormalized(),
			true, bundle.getParameter(paramSustainlevel)->getValueNormalized(), 
			bundle.getParameter(paramHoldlevel)->getValueNormalized());
	}

	ImVec2 availRest = ImGui::GetContentRegionAvail();
	availRest.x *= 2.f / 3.f;
	ImGui::PlotLines("##AHDSR", points, nPoints, 0, 0, 0.f, 1.f, availRest);

	ImGui::EndChild();

}

static void renderLFO(PluginBase& plugin, ImVec2 size = ImGui::GetContentRegionAvail(), unsigned int paramLFOSpeed = 8u, unsigned int paramLFOAmount = 9u, unsigned int paramLFOWaveform = 10u,
	unsigned int paramLFOTemposync = 11u, unsigned int paramLFOphase = 13, unsigned int paramLFObeatMultiplier = 14, unsigned int paramLFObaseline = 15u)
{
	ImGui::BeginChild("LFOsub", size, true);

	const unsigned int nPoints = 250;
	float points[250];
	bool renderLFO = false;
	float bpm = 0.f;
	unsigned int position = 0u;
	PluginBase* ptr = nullptr;
	PluginParameterBundle const& bundle = plugin.getParameters();

	if (ptr = dynamic_cast<PluginBase*>(&plugin), ptr)
	{
		if (ptr->getBPMandPosition(bpm, position))
		{
			renderLFO = true;
		}
	}

	ImGui::PushID(paramLFOAmount);
	renderParam(bundle.getParameter(paramLFOAmount), plugin, paramLFOAmount);
	ImGui::PopID();

	ImGui::PushID(paramLFOWaveform);
	renderParam(bundle.getParameter(paramLFOWaveform), plugin, paramLFOWaveform);
	ImGui::PopID();

	ImGui::PushID(paramLFOTemposync);
	renderParam(bundle.getParameter(paramLFOTemposync), plugin, paramLFOTemposync);
	ImGui::PopID();

	BoolParameter const* temposync = dynamic_cast<BoolParameter const*>(bundle.getParameter(paramLFOTemposync));
	if (temposync->getValue() == 0)
	{
		ImGui::PushID(paramLFOSpeed);
		renderParam(bundle.getParameter(paramLFOSpeed), plugin, paramLFOSpeed);
		ImGui::PopID();

	}
	else
	{
		ImGui::PushID(paramLFObeatMultiplier);
		renderParam(bundle.getParameter(paramLFObeatMultiplier), plugin, paramLFObeatMultiplier);
		ImGui::PopID();
		// beat multiply
	}

	ImGui::PushID(paramLFOphase);
	renderParam(bundle.getParameter(paramLFOphase), plugin, paramLFOphase);
	ImGui::PopID();

	// baseline
	ImGui::PushID(paramLFObaseline);
	renderParam(bundle.getParameter(paramLFObaseline), plugin, paramLFObaseline);
	ImGui::PopID();
	// param LF target
	//range = bundle.getParameter(paramLFOtarget);
	//min = range->getStart();
	//max = range->getEnd();
	//skew = range->getSkew();
	//
	//float unnormalizedLFOtarget = bundle.getParameterUnnormalized(paramLFOtarget);
	//if (ImGui::DragFloat("lfoTarget", &unnormalizedLFOtarget, 0.01f, min * 1.005, max * 0.995, "%.3f", 1 / skew))
	//	plugin.setParameterAutomated(paramLFOtarget, range->toNormalized(unnormalizedLFOtarget));





	//// Plot lfo
	//float length = 60.f * 1000.f / bpm; // milliseconds per beat
	//std::string lengthStr = "Length of visualized curve: " + std::to_string(length) + " ms.";
	//ImGui::Text(lengthStr.c_str());
	//
	//
	////if (length < 1000.f)
	////	length = 1000.f;
	////else if (length > 5000.f)
	////	length = 5000.f;
	//float incrementForVisualization = length / static_cast<float>(nPoints);
	//VoiceState dummy;
	//dummy.on(0u, 69u, 100u);
	//for (unsigned int i = 0; i < nPoints; i++)
	//{
	//	points[i] = 1.f;
	//
	//	performAHDSR<float>(points, dummy, aux::millisecToSamples(i * incrementForVisualization), i,
	//		unnormalizedAttack, unnormalizedRelease, unnormalizedHold, unnormalizedDecay, unnormalizedSustain,
	//		true, normalizedSustainlevel, normalizedHoldlevel);	}
	//
	//ImVec2 availRest = ImGui::GetContentRegionAvail();
	//availRest.x *= 2.f / 3.f;
	//ImGui::PlotLines("##AHDSR", points, nPoints, 0, 0, 0.f, 1.f, availRest);

	ImGui::EndChild();

}
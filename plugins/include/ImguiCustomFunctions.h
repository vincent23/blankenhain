#pragma once
#include <imgui.h>
#include "FloatParameter.h"
#include "PluginBase.h"
#include "VoiceState.h"
#include "InstrumentBase.h"
#include "Oscillators.h"

static void renderParam(PluginBase& plugin, unsigned int paramIndex, float paramDragSpeed = 1.f)
{
	const PluginParameterBundle& bundle = plugin.getParameters();
	FloatParameter const* param = bundle.getParameter(paramIndex);

	ImGui::PushID(paramIndex);
	if (dynamic_cast<BoolParameter const*>(param))
	{
		BoolParameter const* cParam = dynamic_cast<BoolParameter const*>(param);
		const int before = cParam->getValue() ? 1 : 0;
		int after = cParam->getValue() ? 1 : 0;

		ImGui::Text(cParam->getName().c_str()); ImGui::SameLine();
		ImGui::RadioButton("Off", &after, 0); ImGui::SameLine();
		ImGui::RadioButton("On", &after, 1);
		if (before != after)
			plugin.setParameterAutomated(paramIndex, static_cast<float>(after));

	}
	else if (dynamic_cast<OptionParameter const*>(param))
	{
		OptionParameter const* cParam = dynamic_cast<OptionParameter const*>(param);
		const int before(cParam->getCurrentNumber());
		int current(cParam->getCurrentNumber());
		ImGui::Text(cParam->getName().c_str()); ImGui::SameLine();
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
		float min = 0.f;
		float max = 0.f;
		float skew = 0.f;

		const NormalizedRange& range = param->getRange();
		min = range.getStart();
		max = range.getEnd();
		skew = range.getSkew();
		float unnormalized = param->getValueUnnormalized();
		
		float normalizedParamDragSpeed = paramDragSpeed * 0.001f / 2.f * (max - min); 
		// Relative to range, if paramDragSpeed == 1 && (max - min) == 2, then legacy bh2 "default" behaviour is yielded.
		
		if (ImGui::DragFloat(param->getName().c_str(), &unnormalized, normalizedParamDragSpeed, min * 1.002f, max * 0.998f, "%.3f", 1.f / skew))
			plugin.setParameterAutomated(paramIndex, range.toNormalized(unnormalized));

		ImGui::SameLine();
		if (ImGui::Button("Reset"))
		{
			plugin.setParameterAutomated(paramIndex, param->getDefaultValueNormalized());
		}
	
	}
	ImGui::PopID();
}


static void renderADHSR(PluginBase& plugin, ImVec2 const& size = ImGui::GetContentRegionAvail(), TempoData const *const tempoData = nullptr,
	unsigned int paramAttack = 0u, unsigned int paramHold = 1u, unsigned int paramHoldlevel = 2u,
	unsigned int paramDecay = 3u, unsigned int paramSustainbool = 4u, unsigned int paramSustain = 5u,
	unsigned int paramSustainlevel = 6u, unsigned int paramRelease = 7u)
{
	ImGui::BeginChild("adsrsub", size, true);
	constexpr unsigned int nPoints = 250;
	float points[nPoints], beats[nPoints];

	PluginParameterBundle const& bundle = plugin.getParameters();

	renderParam(plugin, paramAttack);
	renderParam(plugin, paramHold);
	renderParam(plugin, paramHoldlevel);
	renderParam(plugin, paramDecay);
	renderParam(plugin, paramSustainbool);
	renderParam(plugin, paramSustain);
	renderParam(plugin, paramSustainlevel);
	renderParam(plugin, paramRelease);

	// Plot adhsr
	float length = bundle.getParameter(paramAttack)->getValueUnnormalized() 
		+ bundle.getParameter(paramHold)->getValueUnnormalized()
		+ bundle.getParameter(paramDecay)->getValueUnnormalized()
		+ bundle.getParameter(paramSustain)->getValueUnnormalized()
		+ bundle.getParameter(paramRelease)->getValueUnnormalized();




	if (length < 1000.f)
		length = 1000.f;
	else if (length > 5000.f)
		length = 5000.f;
	std::string lengthStr = "Length of visualized curve: " + std::to_string(length) + " ms.";
	ImGui::Text(lengthStr.c_str());


	const float incrementForVisualization = length / static_cast<float>(nPoints);
	VoiceState dummy;
	dummy.on(0u, 69u, 100u);
	for (unsigned int i = 0; i < nPoints; i++)
	{
		points[i] = 1.f;
		performAHDSR<float>(points, dummy, static_cast<unsigned int>(aux::millisecToSamples(i * incrementForVisualization)),
			i,
			bundle.getParameter(paramAttack)->getValueUnnormalized(), 
			bundle.getParameter(paramRelease)->getValueUnnormalized(), 
			bundle.getParameter(paramHold)->getValueUnnormalized(), 
			bundle.getParameter(paramDecay)->getValueUnnormalized(), 
			bundle.getParameter(paramSustain)->getValueUnnormalized(),
			true, 
			bundle.getParameter(paramSustainlevel)->getValueNormalized(), 
			bundle.getParameter(paramHoldlevel)->getValueNormalized());
	}

	ImVec2 availRest = ImGui::GetContentRegionAvail();
	availRest.x *= 2.f / 3.f;

	// We paint dashes at beats if tempodata is supplied
	if (tempoData != nullptr)
	{
		if (tempoData->usesTempoData)
		{
			beats[0u] = 1.f;
			for (unsigned int i = 1; i < nPoints; i++)
			{
				const volatile float currentMs = i * incrementForVisualization;
				beats[i] = 0.f;
				const volatile float beatEverySoManyMs = 60000.f / tempoData->bpm;
				const volatile float currentMsAfterBeat = std::fmod(currentMs, beatEverySoManyMs);
				if (currentMsAfterBeat < incrementForVisualization)
					beats[i] = 1.f;
				else if (std::fmod(currentMs, beatEverySoManyMs / 4.f) < incrementForVisualization)
					beats[i] = 0.35f;
			}
			ImGui::PlotLines("##AHDSR", points, nPoints, 0, 0, 0.f, 1.f, ImVec2(availRest.x, availRest.y * 3.f / 4.f ));
			ImGui::PlotHistogram("##Tempo", beats, nPoints, 0, NULL, 0.0f, 1.0f, ImVec2(availRest.x, availRest.y / 4.f));
		}
			else
	{
		ImGui::PlotLines("##AHDSR", points, nPoints, 0, 0, 0.f, 1.f, availRest);
	}
	}
	else
	{
		ImGui::PlotLines("##AHDSR", points, nPoints, 0, 0, 0.f, 1.f, availRest);
	}


	ImGui::EndChild();

}


static void renderLFO(PluginBase& plugin, CommonLFO lfo, TempoData const & tempoData, ImVec2 size = ImGui::GetContentRegionAvail(),
	unsigned int paramLFOSpeed = 8u, unsigned int paramLFOAmount = 9u, unsigned int paramLFOWaveform = 10u,
	unsigned int paramLFOTemposync = 11u, unsigned int paramLFOphase = 13, unsigned int paramLFObeatMultiplier = 14, 
	unsigned int paramLFObaseline = 15u, int paramLFOtarget = -1)
{
	ImGui::BeginChild("LFOsub", size, true);


	PluginParameterBundle const& bundle = plugin.getParameters();

	renderParam(plugin, paramLFOAmount);
	renderParam(plugin, paramLFOWaveform);
	renderParam(plugin, paramLFOTemposync);

	BoolParameter const* temposync = dynamic_cast<BoolParameter const*>(bundle.getParameter(paramLFOTemposync));

#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
#include "../../libblankenhain/include/warnings.h"
	if (temposync == nullptr)
		throw "Temposync is nullptr!";
#endif
	if (temposync->getValue() == 0)
	{
		renderParam(plugin, paramLFOSpeed);
	}
	else
	{
		renderParam(plugin, paramLFObeatMultiplier);
	}
	renderParam(plugin, paramLFOphase);
	if (paramLFObaseline != -1)
	{
		renderParam(plugin, paramLFObaseline);
	}

	if (paramLFOtarget != -1)
	{
		renderParam(plugin, paramLFOtarget);
	}
	//
	//
	// Plot lfo
	constexpr unsigned int nPoints = 250;
	float points[nPoints], beats[nPoints];
	float length;
	if (tempoData.usesTempoData)
		length = 60.f * 1000.f / tempoData.bpm; // milliseconds per beat
	else
		length = 60.f * 1000.f / 120;
	std::string lengthStr = "Length of visualized curve: " + std::to_string(static_cast<unsigned int>(length)) + " ms.";
	ImGui::Text(lengthStr.c_str());
	
	
	if (length < 1000.f)
		length = 1000.f;
	else if (length > 5000.f)
		length = 5000.f;
	const float incrementForVisualization = length / static_cast<float>(nPoints);

	for (unsigned int i = 0; i < nPoints; i++)
	{
		float lfoVal = lfo.getSample(aux::millisecToSamples(static_cast<float>(i) * incrementForVisualization));
		lfoVal = (lfoVal + 1.f) / 2.f;
		//bundle.getParameter(lfoAmount)
		points[i] = lfoVal;

		//draw
	}
	
	ImVec2 availRest = ImGui::GetContentRegionAvail();
	availRest.x *= 2.f / 3.f;
	
	// This paints the bars and ticks, if tempodate is supplied
	// Is tempoData supplied?
	if (tempoData.usesTempoData)
	{
		beats[0u] = 1.f;
		for (unsigned int i = 1; i < nPoints; i++)
		{
			const volatile float currentMs = i * incrementForVisualization;
			beats[i] = 0.f;
			const volatile float beatEverySoManyMs = 60000.f / tempoData.bpm;
			const volatile float currentMsAfterBeat = std::fmod(currentMs, beatEverySoManyMs);
			if (currentMsAfterBeat < incrementForVisualization)
				beats[i] = 1.f;
			else if (std::fmod(currentMs, beatEverySoManyMs / 4.f) < incrementForVisualization)
				beats[i] = 0.35f;
		}


		ImGui::PlotLines("##LFO", points, nPoints, 0, 0, 0.f, 1.f, ImVec2(availRest.x, availRest.y * 3.f / 4.f));

		ImGui::PlotHistogram("##Tempo", beats, nPoints, 0, NULL, 0.0f, 1.0f, ImVec2(availRest.x, availRest.y / 4.f));
	}
	else
	{
		ImGui::PlotLines("##LFO", points, nPoints, 0, 0, 0.f, 1.f, availRest);
	}



	//
	ImGui::EndChild();

}

#pragma once
#include <imgui.h>

static void renderADHSR(PluginBase& plugin, ImVec2 size = ImGui::GetContentRegionAvail(), unsigned int paramAttack = 0u, unsigned int paramHold = 1u, unsigned int paramHoldlevel = 2u,
	unsigned int paramDecay = 3u, unsigned int paramSustainbool = 4u, unsigned int paramSustain = 5u
	, unsigned int paramSustainlevel = 6u, unsigned int paramRelease = 7u)
{
	ImGui::BeginChild("adsrsub", size, true);
	const unsigned int nPoints = 250;
	float points[250];

	const NormalizedRange* range = nullptr;
	float min = 0.f;
	float max = 0.f;
	float skew = 0.f;

	// attack
	PluginParameterBundle const& bundle = plugin.getParameters();
	range = bundle.getParameter(paramAttack);
	min = range->getStart();
	max = range->getEnd();
	skew = range->getSkew();

	float unnormalizedAttack = bundle.getParameterUnnormalized(paramAttack);
	if (ImGui::DragFloat("attack", &unnormalizedAttack, 0.01f, min * 1.005, max * 0.995, "%.3f", 1 / skew))
		plugin.setParameterAutomated(paramAttack, range->toNormalized(unnormalizedAttack));

	// hold
	range = bundle.getParameter((paramHold));
	min = range->getStart();
	max = range->getEnd();
	skew = range->getSkew();

	float unnormalizedHold = bundle.getParameterUnnormalized(paramHold);
	if (ImGui::DragFloat("hold", &unnormalizedHold, 0.01f, min * 1.005, max * 0.995, "%.3f", 1 / skew))
		plugin.setParameterAutomated(paramHold, range->toNormalized(unnormalizedHold));

	// holdLevel
	range  =bundle.getParameter((paramHold));
	min = range->getStart();
	max = range->getEnd();
	skew = range->getSkew();

	float normalizedHoldlevel = bundle.getParameterUnnormalized(paramHoldlevel);
	if (ImGui::DragFloat("holdLevel", &normalizedHoldlevel, 0.01f, min * 1.005, max * 0.995, "%.3f", 1 / skew))
		plugin.setParameterAutomated(paramHoldlevel, range->toNormalized(normalizedHoldlevel));


	// decay
	range = bundle.getParameter(paramDecay);
	min = range->getStart();
	max = range->getEnd();
	skew = range->getSkew();

	float unnormalizedDecay = bundle.getParameterUnnormalized(paramDecay);
	if (ImGui::DragFloat("decay", &unnormalizedDecay, 0.01f, min * 1.005, max * 0.995, "%.3f", 1 / skew))
		plugin.setParameterAutomated(paramDecay, range->toNormalized(unnormalizedDecay));


	// sustainBool
	range = bundle.getParameter(paramSustainbool);
	min = range->getStart();
	max = range->getEnd();
	skew = range->getSkew();
	int unnormalizedSustainbool = bundle.getParameterUnnormalized(paramSustainbool);
	ImGui::Text("Hard Sustain Enforcement"); ImGui::SameLine();
	ImGui::RadioButton("On", &unnormalizedSustainbool, 0); ImGui::SameLine();
	ImGui::RadioButton("Off", &unnormalizedSustainbool, 1);
	if (unnormalizedSustainbool != static_cast<int>(bundle.getParameterUnnormalized(paramSustainbool)))
		plugin.setParameterAutomated(paramSustainbool, range->toNormalized(unnormalizedSustainbool));

	// sustain
	range = bundle.getParameter(paramSustain);
	min = range->getStart();
	max = range->getEnd();
	skew = range->getSkew();

	float unnormalizedSustain = bundle.getParameterUnnormalized(paramSustain);
	if (ImGui::DragFloat("sustain", &unnormalizedSustain, 0.01f, min * 1.005, max * 0.995, "%.3f", 1 / skew))
		plugin.setParameterAutomated(paramSustain, range->toNormalized(unnormalizedSustain));

	// sustainLevel
	range = bundle.getParameter(paramSustainlevel);
	min = range->getStart();
	max = range->getEnd();
	skew = range->getSkew();

	float normalizedSustainlevel = bundle.getParameterUnnormalized(paramSustainlevel);
	if (ImGui::DragFloat("sustainLevel", &normalizedSustainlevel, 0.01f, min * 1.005, max * 0.995, "%.3f", 1 / skew))
		plugin.setParameterAutomated(paramSustainlevel, range->toNormalized(normalizedSustainlevel));


	// release
	range = bundle.getParameter(paramRelease);
	min = range->getStart();
	max = range->getEnd();
	skew = range->getSkew();

	float unnormalizedRelease = bundle.getParameterUnnormalized(paramRelease);
	if (ImGui::DragFloat("release", &unnormalizedRelease, 0.01f, min * 1.005, max * 0.995, "%.3f", 1 / skew))
		plugin.setParameterAutomated(paramRelease, range->toNormalized(unnormalizedRelease));


	// Plot adhsr
	float length = unnormalizedAttack + unnormalizedHold + unnormalizedDecay + unnormalizedSustain + unnormalizedRelease;
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
			unnormalizedAttack, unnormalizedRelease, unnormalizedHold, unnormalizedDecay, unnormalizedSustain,
			true, normalizedSustainlevel, normalizedHoldlevel);
	}

	ImVec2 availRest = ImGui::GetContentRegionAvail();
	availRest.x *= 2.f / 3.f;
	ImGui::PlotLines("##AHDSR", points, nPoints, 0, 0, 0.f, 1.f, availRest);

	ImGui::EndChild();

}


static void renderLFO(PluginBase& plugin, ImVec2 size = ImGui::GetContentRegionAvail(), unsigned int paramLFOSpeed = 8u, unsigned int paramLFOAmount = 9u, unsigned int paramLFOWaveform = 10u,
	unsigned int paramLFOTemposync = 11u, unsigned int paramLFOphase = 13, unsigned int paramLFObeatMultiplier = 14)
{
	ImGui::BeginChild("LFOsub", size, true);
	//const unsigned int nPoints = 250;
	//float points[250];

	const NormalizedRange* range = nullptr;
	float min = 0.f;
	float max = 0.f;
	float skew = 0.f;


	PluginParameterBundle const& bundle = plugin.getParameters();


	// Amount
	range = bundle.getParameter((paramLFOAmount));
	min = range->getStart();
	max = range->getEnd();
	skew = range->getSkew();

	float unnormalizedLFOAmount = bundle.getParameterUnnormalized(paramLFOAmount);
	if (ImGui::DragFloat("lfoAmount", &unnormalizedLFOAmount, 0.01f, min * 1.005, max * 0.995, "%.3f", 1 / skew))
		plugin.setParameterAutomated(paramLFOAmount, range->toNormalized(unnormalizedLFOAmount));

	// waveform
	range = bundle.getParameter((paramLFOWaveform));
	min = range->getStart();
	max = range->getEnd();
	skew = range->getSkew();

	float unnormalizedLFOWaveform = bundle.getParameterUnnormalized(paramLFOWaveform);
	if (ImGui::DragFloat("lfoWaveform", &unnormalizedLFOWaveform, 0.01f, min * 1.005, max * 0.995, "%.3f", 1 / skew))
		plugin.setParameterAutomated(paramLFOWaveform, range->toNormalized(unnormalizedLFOWaveform));


	// paramLFOTemposync
	range = bundle.getParameter(paramLFOTemposync);
	min = range->getStart();
	max = range->getEnd();
	skew = range->getSkew();
	int unnormalizedLFOTemposync = bundle.getParameterUnnormalized(paramLFOTemposync);
	ImGui::Text("Temposync"); ImGui::SameLine();
	ImGui::RadioButton("Off", &unnormalizedLFOTemposync, 0); ImGui::SameLine();
	ImGui::RadioButton("On", &unnormalizedLFOTemposync, 1);
	if (unnormalizedLFOTemposync != static_cast<int>(bundle.getParameterUnnormalized(paramLFOTemposync)))
		plugin.setParameterAutomated(paramLFOTemposync, range->toNormalized(unnormalizedLFOTemposync));

	if (unnormalizedLFOTemposync == 0)
	{
		// Speed
		range = bundle.getParameter(paramLFOSpeed);
		min = range->getStart();
		max = range->getEnd();
		skew = range->getSkew();

		float unnormalizedLFOSpeed = bundle.getParameterUnnormalized(paramLFOSpeed);
		if (ImGui::DragFloat("lfoSpeed", &unnormalizedLFOSpeed, 0.01f, min * 1.005, max * 0.995, "%.3f", 1 / skew))
			plugin.setParameterAutomated(paramLFOSpeed, range->toNormalized(unnormalizedLFOSpeed));
	}
	else
	{
		// beat multiply
		range = bundle.getParameter(paramLFObeatMultiplier);
		min = range->getStart();
		max = range->getEnd();
		skew = range->getSkew();

		float unnormalizedLFObeatMultiplier = bundle.getParameterUnnormalized(paramLFObeatMultiplier);
		if (ImGui::DragFloat("lfoBeatMult", &unnormalizedLFObeatMultiplier, 0.01f, min * 1.005, max * 0.995, "%.3f", 1 / skew))
			plugin.setParameterAutomated(paramLFObeatMultiplier, range->toNormalized(unnormalizedLFObeatMultiplier));
	}

	// phase
	range = bundle.getParameter((paramLFOphase));
	min = range->getStart();
	max = range->getEnd();
	skew = range->getSkew();

	float unnormalizedLFOphase = bundle.getParameterUnnormalized(paramLFOphase);
	if (ImGui::DragFloat("lfoPhase", &unnormalizedLFOphase, 0.01f, min * 1.0025, max * 0.9975, "%.3f", 1 / skew))
		plugin.setParameterAutomated(paramLFOphase, range->toNormalized(unnormalizedLFOphase));

	// param LF target
	//range = bundle.getParameter(paramLFOtarget);
	//min = range->getStart();
	//max = range->getEnd();
	//skew = range->getSkew();
	//
	//float unnormalizedLFOtarget = bundle.getParameterUnnormalized(paramLFOtarget);
	//if (ImGui::DragFloat("lfoTarget", &unnormalizedLFOtarget, 0.01f, min * 1.005, max * 0.995, "%.3f", 1 / skew))
	//	plugin.setParameterAutomated(paramLFOtarget, range->toNormalized(unnormalizedLFOtarget));





	// Plot lfo
	//float length = unnormalizedAttack + unnormalizedHold + unnormalizedDecay + unnormalizedSustain + unnormalizedRelease;
	//std::string lengthStr = "Length of visualized curve: " + std::to_string(length) + " ms.";
	//ImGui::Text(lengthStr.c_str());
	//
	//
	//if (length < 1000.f)
	//	length = 1000.f;
	//else if (length > 5000.f)
	//	length = 5000.f;
	//float incrementForVisualization = length / static_cast<float>(nPoints);
	//VoiceState dummy;
	//dummy.on(0u, 69u, 100u);
	//for (unsigned int i = 0; i < nPoints; i++)
	//{
	//	points[i] = 1.f;
	//	performAHDSR<float>(points, dummy, aux::millisecToSamples(i * incrementForVisualization), i,
	//		unnormalizedAttack, unnormalizedRelease, unnormalizedHold, unnormalizedDecay, unnormalizedSustain,
	//		true, normalizedSustainlevel, normalizedHoldlevel);
	//}
	//
	//ImVec2 availRest = ImGui::GetContentRegionAvail();
	//availRest.x *= 2.f / 3.f;
	//ImGui::PlotLines("##AHDSR", points, nPoints, 0, 0, 0.f, 1.f, availRest);

	ImGui::EndChild();

}
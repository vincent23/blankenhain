#pragma once
#include <imgui.h>

static void renderADHSR(PluginBase& plugin, ImVec2 size = ImGui::GetContentRegionAvail(), unsigned int paramAttack = 0u, unsigned int paramHold = 1u, unsigned int paramHoldlevel = 2u,
	unsigned int paramDecay = 3u, unsigned int paramSustainbool = 4u, unsigned int paramSustain = 5u
	, unsigned int paramSustainlevel = 6u, unsigned int paramRelease = 7u)
{
	ImGui::BeginChild("adsrsub", size, true);
	const unsigned int nPoints = 250;
	float points[250];

	NormalizedRange* range = nullptr;
	float min = 0.f;
	float max = 0.f;
	float skew = 0.f;

	// attack
	PluginParameterBundle const& bundle = plugin.getParameters();
	range = bundle.getParameterBundle().getParameter(paramAttack);
	min = range->getStart();
	max = range->getEnd();
	skew = range->getSkew();

	float unnormalizedAttack = bundle.getParameterUnnormalized(paramAttack);
	if (ImGui::DragFloat("attack", &unnormalizedAttack, 0.01f, min * 1.005, max * 0.995, "%.3f", 1 / skew))
		plugin.setParameterAutomated(paramAttack, range->toNormalized(unnormalizedAttack));

	// hold
	range = bundle.getParameterBundle().getParameter(paramHold);
	min = range->getStart();
	max = range->getEnd();
	skew = range->getSkew();

	float unnormalizedHold = bundle.getParameterUnnormalized(paramHold);
	if (ImGui::DragFloat("hold", &unnormalizedHold, 0.01f, min * 1.005, max * 0.995, "%.3f", 1 / skew))
		plugin.setParameterAutomated(paramHold, range->toNormalized(unnormalizedHold));

	// holdLevel
	range = bundle.getParameterBundle().getParameter(paramHoldlevel);
	min = range->getStart();
	max = range->getEnd();
	skew = range->getSkew();

	float normalizedHoldlevel = bundle.getParameterUnnormalized(paramHoldlevel);
	if (ImGui::DragFloat("holdLevel", &normalizedHoldlevel, 0.01f, min * 1.005, max * 0.995, "%.3f", 1 / skew))
		plugin.setParameterAutomated(paramHoldlevel, range->toNormalized(normalizedHoldlevel));


	// decay
	range = bundle.getParameterBundle().getParameter(paramDecay);
	min = range->getStart();
	max = range->getEnd();
	skew = range->getSkew();

	float unnormalizedDecay = bundle.getParameterUnnormalized(paramDecay);
	if (ImGui::DragFloat("decay", &unnormalizedDecay, 0.01f, min * 1.005, max * 0.995, "%.3f", 1 / skew))
		plugin.setParameterAutomated(paramDecay, range->toNormalized(unnormalizedDecay));


	// sustainBool
	range = bundle.getParameterBundle().getParameter(paramSustainbool);
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
	range = bundle.getParameterBundle().getParameter(paramSustain);
	min = range->getStart();
	max = range->getEnd();
	skew = range->getSkew();

	float unnormalizedSustain = bundle.getParameterUnnormalized(paramSustain);
	if (ImGui::DragFloat("sustain", &unnormalizedSustain, 0.01f, min * 1.005, max * 0.995, "%.3f", 1 / skew))
		plugin.setParameterAutomated(paramSustain, range->toNormalized(unnormalizedSustain));

	// sustainLevel
	range = bundle.getParameterBundle().getParameter(paramSustainlevel);
	min = range->getStart();
	max = range->getEnd();
	skew = range->getSkew();

	float normalizedSustainlevel = bundle.getParameterUnnormalized(paramSustainlevel);
	if (ImGui::DragFloat("sustainLevel", &normalizedSustainlevel, 0.01f, min * 1.005, max * 0.995, "%.3f", 1 / skew))
		plugin.setParameterAutomated(paramSustainlevel, range->toNormalized(normalizedSustainlevel));


	// release
	range = bundle.getParameterBundle().getParameter(paramRelease);
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
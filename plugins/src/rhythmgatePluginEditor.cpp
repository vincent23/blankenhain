#include "RhythmgatePluginEditor.h"

#include "constants.h"
#include "NormalizedRange.h"
#include "PluginParameterBundle.h"
#include "ParameterBundle.h"
#include "AuxFunc.h"
#include "RhythmgateEffect.h"

#include <imgui.h>
#include <cmath>
#include <complex>

#include <windows.h>

RhythmgatePluginEditor::RhythmgatePluginEditor(PluginBase* plugin)
	: ImguiEffectEditor(plugin, 700, 500)
{}

void RhythmgatePluginEditor::imguiFrame()
{
	// some hacky gui code
	// don't use as an example

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	const PluginParameterBundle& bundle = plugin.getParameters();
	static int item = 0;

	const NormalizedRange& range = bundle.getParameter(0u)->getRange();
	float min = range.getStart();
	float max = range.getEnd();
	float skew = range.getSkew();

	static float unnormalized0 = bundle.getParameterUnnormalized(0u);
	if (ImGui::DragFloat("slider0", &unnormalized0, 0.05f, min, max, "%.3f", 1 / skew))
		plugin.setParameterAutomated(0u, range.toNormalized(unnormalized0));

	static float unnormalized1 = bundle.getParameterUnnormalized(1u);
	if (ImGui::DragFloat("slider1", &unnormalized1, 0.05f, min, max, "%.3f", 1 / skew))
		plugin.setParameterAutomated(1u, range.toNormalized(unnormalized1));

	static float unnormalized2 = bundle.getParameterUnnormalized(2u);
	if (ImGui::DragFloat("slider2", &unnormalized2, 0.05f, min, max, "%.3f", 1 / skew))
		plugin.setParameterAutomated(2u, range.toNormalized(unnormalized2));

	static float unnormalized3 = bundle.getParameterUnnormalized(3u);
	if (ImGui::DragFloat("slider3", &unnormalized3, 0.05f, min, max, "%.3f", 1 / skew))
		plugin.setParameterAutomated(3u, range.toNormalized(unnormalized3));

	static float unnormalized4 = bundle.getParameterUnnormalized(4u);
	if (ImGui::DragFloat("slider4", &unnormalized4, 0.05f, min, max, "%.3f", 1 / skew))
		plugin.setParameterAutomated(4u, range.toNormalized(unnormalized4));

	static float unnormalized5 = bundle.getParameterUnnormalized(5u);
	if (ImGui::DragFloat("slider5", &unnormalized5, 0.05f, min, max, "%.3f", 1 / skew))
		plugin.setParameterAutomated(5u, range.toNormalized(unnormalized5));

	static float unnormalized6 = bundle.getParameterUnnormalized(6u);
	if (ImGui::DragFloat("slider6", &unnormalized6, 0.05f, min, max, "%.3f", 1 / skew))
		plugin.setParameterAutomated(6u, range.toNormalized(unnormalized6));

	static float unnormalized7 = bundle.getParameterUnnormalized(7u);
	if (ImGui::DragFloat("slider7", &unnormalized7, 0.05f, min, max, "%.3f", 1 / skew))
		plugin.setParameterAutomated(7u, range.toNormalized(unnormalized7));

	static float unnormalized8 = bundle.getParameterUnnormalized(8u);
	if (ImGui::DragFloat("slider8", &unnormalized8, 0.05f, min, max, "%.3f", 1 / skew))
		plugin.setParameterAutomated(8u, range.toNormalized(unnormalized8));

	static float unnormalized9 = bundle.getParameterUnnormalized(9u);
	if (ImGui::DragFloat("slider9", &unnormalized9, 0.05f, min, max, "%.3f", 1 / skew))
		plugin.setParameterAutomated(9u, range.toNormalized(unnormalized9));

	static float unnormalized10 = bundle.getParameterUnnormalized(10u);
	if (ImGui::DragFloat("slider10", &unnormalized10, 0.05f, min, max, "%.3f", 1 / skew))
		plugin.setParameterAutomated(10u, range.toNormalized(unnormalized10));

	static float unnormalized11 = bundle.getParameterUnnormalized(11u);
	if (ImGui::DragFloat("slider11", &unnormalized11, 0.05f, min, max, "%.3f", 1 / skew))
		plugin.setParameterAutomated(11u, range.toNormalized(unnormalized11));

	static float unnormalized12 = bundle.getParameterUnnormalized(12u);
	if (ImGui::DragFloat("slider12", &unnormalized12, 0.05f, min, max, "%.3f", 1 / skew))
		plugin.setParameterAutomated(12u, range.toNormalized(unnormalized12));

	static float unnormalized13 = bundle.getParameterUnnormalized(13u);
	if (ImGui::DragFloat("slider13", &unnormalized13, 0.05f, min, max, "%.3f", 1 / skew))
		plugin.setParameterAutomated(13u, range.toNormalized(unnormalized13));

	static float unnormalized14 = bundle.getParameterUnnormalized(14u);
	if (ImGui::DragFloat("slider14", &unnormalized14, 0.05f, min, max, "%.3f", 1 / skew))
		plugin.setParameterAutomated(14u, range.toNormalized(unnormalized14));

	static float unnormalized15 = bundle.getParameterUnnormalized(15u);
	if (ImGui::DragFloat("slider15", &unnormalized15, 0.05f, min, max, "%.3f", 1 / skew))
		plugin.setParameterAutomated(15u, range.toNormalized(unnormalized15));

	const NormalizedRange& range2 = bundle.getParameter(16u)->getRange();
	min = range2.getStart();
	max = range2.getEnd();
	skew = range2.getSkew();
	static float attack = bundle.getParameterUnnormalized(16u);
	if (ImGui::DragFloat("attack", &attack, 0.01f, min, max, "%.3f", 1 / skew))
		plugin.setParameterAutomated(16u, range2.toNormalized(attack));

	const NormalizedRange& range3 = bundle.getParameter(17u)->getRange();
	min = range3.getStart();
	max = range3.getEnd();
	skew = range3.getSkew();
	static float release = bundle.getParameterUnnormalized(17u);
	if (ImGui::DragFloat("release", &release, 0.01f, min, max, "%.3f", 1 / skew))
		plugin.setParameterAutomated(17u, range3.toNormalized(release));

	const NormalizedRange& range4 = bundle.getParameter(18u)->getRange();
	min = range4.getStart();
	max = range4.getEnd();
	skew = range4.getSkew();
	static float multiplier = bundle.getParameterUnnormalized(18u);
	if (ImGui::DragFloat("multiplier", &multiplier, 0.01f, min, max, "%.3f", 1 / skew))
	{
		if (multiplier < range4.getStart())
			multiplier = range4.getStart() + 0.0001f;
		plugin.setParameterAutomated(18u, range4.toNormalized(multiplier));
	}

	const NormalizedRange& range5 = bundle.getParameter(19u)->getRange();
	min = range5.getStart();
	max = range5.getEnd();
	skew = range5.getSkew();
	static float offset = bundle.getParameterUnnormalized(19u);
	if (ImGui::DragFloat("offset", &offset, 0.01f, min, max, "%.3f", 1 / skew))
		plugin.setParameterAutomated(19u, range5.toNormalized(offset));

	ImGui::End();
}
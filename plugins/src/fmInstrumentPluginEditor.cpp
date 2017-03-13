#include "fmInstrumentPluginEditor.h"



FmInstrumentPluginEditor::FmInstrumentPluginEditor(PluginBase* plugin)
	: ImguiEffectEditor(plugin, 1000, 700)
{
}


void FmInstrumentPluginEditor::imguiFrame()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	const PluginParameterBundle& bundle = plugin.getParameters();

	// Top section
	// Left: AHDSR
	// Right: Carrier Waveform and carrier stuff

	renderADHSR(plugin, ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 300));
	ImGui::SameLine();

	ImGui::BeginChild("Carrier", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.48f, 300), true);
	renderParam(plugin, 112);
	renderParam(plugin, 113);
	renderParam(plugin, 114);
	renderParam(plugin, 115);

	ImGui::EndChild();

	// End Top Section
	// Begin Bottom section
	// Left & Right: Modulation Oscillators

	ImGui::BeginChild("Modulation1", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 480), false);

	for (unsigned int i = 0u; i < 4u; i++)
	{
		unsigned int currentParamInt = 8 + i * 13;
		// isOn
		renderParam(plugin, currentParamInt + 7);
		float isOn = bundle.getParameterUnnormalized(currentParamInt + 7);

		//ImGui::SameLine();
		// On / Off
		if (isOn == 1.f)
		{
			// key Sync
			renderParam(plugin, currentParamInt + 11);
			bool keysync = bundle.getParameterUnnormalized(currentParamInt + 11) == 1.f;
			if (!keysync)
			{
				//isLFO
				renderParam(plugin, currentParamInt + 8);
				//tempoSyncOn
				renderParam(plugin, currentParamInt + 9);

				float tempoSyncOn = bundle.getParameterUnnormalized(currentParamInt + 9);

				if (tempoSyncOn == 1.f)
				{
					//tempoSyncMultiplier
					renderParam(plugin, currentParamInt + 10);
				}
				else
					renderParam(plugin, currentParamInt);
			}
			else
			{
				renderParam(plugin, currentParamInt + 12);
			}

			// all the rest
			renderParam(plugin, currentParamInt + 1);
			renderParam(plugin, currentParamInt + 2);
			renderParam(plugin, currentParamInt + 3);
			renderParam(plugin, currentParamInt + 4);
			renderParam(plugin, currentParamInt + 5);
			renderParam(plugin, currentParamInt + 6);

			ImGui::Separator();

		}
	}

	ImGui::EndChild();
	ImGui::SameLine();

	ImGui::BeginChild("Modulation2", ImVec2(0, 480), false);
	for (unsigned int i = 4u; i < 8u; i++)
	{
		unsigned int currentParamInt = 8 + i * 13;
		renderParam(plugin, currentParamInt + 7);
		float isOn = bundle.getParameterUnnormalized(currentParamInt + 7);
		//ImGui::SameLine();
		// On / Off
		if (isOn == 1.f)
		{
			// key Sync
			renderParam(plugin, currentParamInt + 11);
			bool keysync = bundle.getParameterUnnormalized(currentParamInt + 11) == 1.f;
			if (!keysync)
			{
				//isLFO
				renderParam(plugin, currentParamInt + 8);
				//tempoSyncOn
				renderParam(plugin, currentParamInt + 9);

				float tempoSyncOn = bundle.getParameterUnnormalized(currentParamInt + 9);

				if (tempoSyncOn == 1.f)
				{
					//tempoSyncMultiplier
					renderParam(plugin, currentParamInt + 10);
				}
				else
					renderParam(plugin, currentParamInt);
			}
			else
			{
				renderParam(plugin, currentParamInt + 12);
			}

			// all the rest
			renderParam(plugin, currentParamInt + 1);
			renderParam(plugin, currentParamInt + 2);
			renderParam(plugin, currentParamInt + 3);
			renderParam(plugin, currentParamInt + 4);
			renderParam(plugin, currentParamInt + 5);
			renderParam(plugin, currentParamInt + 6);

			ImGui::Separator();
		}
	}

	ImGui::EndChild();
	// missing pop -> causes crash
	// ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
	ImGui::End();
}
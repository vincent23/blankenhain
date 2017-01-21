#pragma once

#include "ImguiEffectEditor.h"
#include "PluginBase.h"
#include "ModulationEnvelope.h"

#include "constants.h"
#include "NormalizedRange.h"
#include "PluginParameterBundle.h"
#include "ParameterBundle.h"
#include "AuxFunc.h"
#include "RhythmgateEffect.h"

#include "polyblepInstrumentPlugin.h"

#include <imgui.h>
#include <cmath>
#include <complex>

#include <windows.h>

#include "ImguiCustomFunctions.h"

class polyblepInstrumentPluginEditor : public ImguiEffectEditor
{
public:
	polyblepInstrumentPluginEditor(PluginBase* plugin);

protected:
	void imguiFrame() override;
};
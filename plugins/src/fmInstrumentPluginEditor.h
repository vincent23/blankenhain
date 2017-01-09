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

#include "fmInstrumentPlugin.h"

#include <imgui.h>
#include <cmath>
#include <complex>

#include <windows.h>

#include "ImguiCustomFunctions.h"

class FmInstrumentPluginEditor : public ImguiEffectEditor
{
public:
	FmInstrumentPluginEditor(PluginBase* plugin);

protected:
	void imguiFrame() override;
};
#pragma once

#include "ImguiEffectEditor.h"
#include "PluginBase.h"

#include "constants.h"
#include "NormalizedRange.h"
#include "PluginParameterBundle.h"
#include "ParameterBundle.h"
#include "AuxFunc.h"

#include "PanPlugin.h"

#include <imgui.h>
#include <cmath>
#include <complex>

#include <windows.h>

#include "ImguiCustomFunctions.h"

class PanPluginEditor : public ImguiEffectEditor
{
public:
	PanPluginEditor(PluginBase* plugin);

protected:
	void imguiFrame() override;
};
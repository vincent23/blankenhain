#pragma once

#include "ImguiEffectEditor.h"
#include "PluginBase.h"

#include "constants.h"
#include "NormalizedRange.h"
#include "PluginParameterBundle.h"
#include "ParameterBundle.h"
#include "AuxFunc.h"

#include "FilterPlugin.h"

#include <imgui.h>
#include <cmath>
#include <complex>

#include <windows.h>

#include "ImguiCustomFunctions.h"
#include "FilterEffect.h"

class FilterPluginEditor : public ImguiEffectEditor
{
public:
	FilterPluginEditor(PluginBase* plugin);

protected:
	void imguiFrame() override;
};
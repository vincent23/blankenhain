#pragma once

#include "ImguiEffectEditor.h"
#include "PluginBase.h"

#include "constants.h"
#include "NormalizedRange.h"
#include "PluginParameterBundle.h"
#include "ParameterBundle.h"
#include "AuxFunc.h"

#include "VolumePlugin.h"

#include <imgui.h>
#include <cmath>
#include <complex>

#include <windows.h>

#include "ImguiCustomFunctions.h"

class VolumePluginEditor : public ImguiEffectEditor
{
public:
	VolumePluginEditor(PluginBase* plugin);

protected:
	void imguiFrame() override;
};
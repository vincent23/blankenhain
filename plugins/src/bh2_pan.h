#pragma once

#include "PluginBase.h"
#include "EffectBase.h"
#include "ParameterBundle.h"

class bh2_pan_effect : public EffectBase
{
public:
	bh2_pan_effect() : EffectBase(2u, 512u)
	{
		ParameterBundle* params = getPointerToParameterBundle();
		(params->getParameter(0)) = new ParameterWithProperties(0.f, NormalizedRange(-50.f, 50.f), "pan", "");
		(params->getParameter(1)) = new ParameterWithProperties(0.f, NormalizedRange(), "mono", "bool");
	}

	void process(Sample* buffer, size_t sampleFrames, size_t numberOfParameters, float* parameters)
	{
		float& panningValue = parameters[0];
		bool mono = parameters[1] > 0.5 ? true : false;

		if (mono)
		{
			for (size_t bufferIteration = 0u; bufferIteration < sampleFrames; bufferIteration++)
			{
				alignas(16) double lr[2];
				buffer[bufferIteration].store_aligned(lr);
				lr[1] = lr[0] * (1.f + (0.0f < panningValue ? 0.0f : panningValue) * 0.02f);
				lr[0] = lr[0] * (1.f - (0.0f < panningValue ? panningValue : 0.0f) * 0.02f);
				buffer[bufferIteration] = load_aligned(lr);
			}
		}
		else
		{
			for (size_t bufferIteration = 0u; bufferIteration < sampleFrames; bufferIteration++)
			{
				alignas(16) double lr[2];
				buffer[bufferIteration].store_aligned(lr);
				lr[0] = lr[0] * (1.f - (0.0f < panningValue ? panningValue : 0.0f) * 0.02f);
				lr[1] = lr[1] * (1.f + (0.0f < panningValue ? 0.0f : panningValue) * 0.02f);
				buffer[bufferIteration] = load_aligned(lr);
			}
		}
	}

};


class BH2_pan : public PluginBase
{
public:
	BH2_pan(audioMasterCallback audioMaster)
		: PluginBase(audioMaster, new bh2_pan_effect)
	{ }

	virtual void open()
	{
		setUniqueID(1991);
		isSynth(false);
	}

	virtual void close()
	{

	}
};
#pragma once

#include "PluginBase.h"
#include "EffectBase.h"
#include "ParameterBundle.h"

#include <algorithm>
#include <iostream>

class bh2_bitcrush_effect : public EffectBase
{
public:
	bh2_bitcrush_effect() : EffectBase(3, 512)
	{
		ParameterBundle* params = getPointerToParameterBundle();
		(params->getParameter(0)) = new ParameterWithProperties(0.5f, NormalizedRange(), "Bitcrush", "");
		(params->getParameter(1)) = new ParameterWithProperties(0.f, NormalizedRange(), "Downsample", "");
		(params->getParameter(2)) = new ParameterWithProperties(100.f, NormalizedRange(0.f, 100.f), "Dry/Wet", "%");
	}

	void process(Sample* buffer, size_t numberOfSamples, size_t numberOfParameters, float* parameters)
	{
		float& bitcrush = parameters[0];
		float& downsample = parameters[1];
		float& drywet = parameters[2];

		int groupedSamples = std::min(static_cast<int>(std::max(1.f, downsample * 100.f)), static_cast<int>(numberOfSamples));
		float bitdepth = 12.f * (1.f - bitcrush) + 1.f * bitcrush;
		int steps = static_cast<int>(exp2(bitdepth));

		for (size_t sample = 0; sample < numberOfSamples - groupedSamples; sample += groupedSamples)
		{
			Sample averagedSample(0., 0.);
			for (int i = 0; i < groupedSamples; i++)
			{
				averagedSample += buffer[i + sample] / Sample((double)groupedSamples);
			}

			Sample discretizedSample = discretize(averagedSample * Sample(static_cast<double>(steps)));
			discretizedSample /= Sample(static_cast<double>(steps));

			for (int i = 0; i < groupedSamples; i++) {
				Sample sampleValue = buffer[i + sample];
				buffer[i + sample] = sampleValue * Sample(1. - drywet) + discretizedSample * Sample(drywet);
			}
		}

		Sample averagedSample(0., 0.);
		for (size_t i = static_cast<size_t>(numberOfSamples / groupedSamples) * groupedSamples; i < numberOfSamples; i++)
		{
			averagedSample += buffer[i] / Sample(static_cast<double>((numberOfSamples % groupedSamples)));
		}

		Sample discretizedSample = discretize(averagedSample * Sample(static_cast<double>(steps)));
		discretizedSample /= Sample(static_cast<double>(steps));

		for (size_t i = static_cast<size_t>(numberOfSamples / groupedSamples) * groupedSamples; i < numberOfSamples; i++)
		{
			Sample sampleValue = buffer[i];
			buffer[i] = sampleValue * Sample(1. - drywet) + discretizedSample * Sample(drywet);
		}
	}

	Sample discretize(Sample const& sample)
	{
		alignas(16) double lr[2];
		sample.store_aligned(lr);
		lr[0] = static_cast<double>(static_cast<int>(lr[0]));
		lr[1] = static_cast<double>(static_cast<int>(lr[1]));
		return load_aligned(lr);
	}

};


class BH2_bitcrush : public PluginBase
{
public:
	BH2_bitcrush(audioMasterCallback audioMaster)
		: PluginBase(audioMaster, new bh2_bitcrush_effect)
	{ }

	virtual void open()
	{
		setUniqueID(189);
		isSynth(false);
	}

	virtual void close()
	{

	}
};
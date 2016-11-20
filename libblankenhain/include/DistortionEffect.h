#pragma once

#include "EffectBase.h"

class DistortionEffect : public EffectBase
{
public:
	DistortionEffect();

	enum distortionAlgorithms
	{
		ArayaAndSuyama = 0, DoidicSymmetric,
		DoidicAsymmetric
	};

	void process(Sample* buffer, size_t numberOfSamples, size_t numberOfParameters, float* parameters);
};
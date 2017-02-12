#include "PanEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"

PanEffect::PanEffect() : EffectBase(2u)
{
	ParameterBundle* params = getPointerToParameterBundle();
	(params->getParameter(0)) = new FloatParameter(0.f, NormalizedRange(-50.f, 50.f), "pan", "");
	(params->getParameter(1)) = new FloatParameter(0.f, NormalizedRange(), "mono", "bool");
}

void PanEffect::process(Sample* buffer, size_t numberOfSamples)
{
	InterpolatedValue<float>& panningValue = getInterpolatedParameter(0);
	bool mono = getInterpolatedParameter(1).get() > 0.5 ? true : false;

	if (mono)
	{
		for (size_t bufferIteration = 0u; bufferIteration < numberOfSamples; bufferIteration++)
		{
			alignas(16) double lr[2];
			buffer[bufferIteration].store_aligned(lr);
			lr[1] = lr[0] * (1.f + (0.0f < panningValue.get() ? 0.0f : panningValue.get()) * 0.02f);
			lr[0] = lr[0] * (1.f - (0.0f < panningValue.get() ? panningValue.get() : 0.0f) * 0.02f);
			buffer[bufferIteration] = Sample::load_aligned(lr);
			nextSample();
		}
	}
	else
	{
		for (size_t bufferIteration = 0u; bufferIteration < numberOfSamples; bufferIteration++)
		{
			alignas(16) double lr[2];
			buffer[bufferIteration].store_aligned(lr);
			lr[0] = lr[0] * (1.f - (0.0f < panningValue.get() ? panningValue.get() : 0.0f) * 0.02f);
			lr[1] = lr[1] * (1.f + (0.0f < panningValue.get() ? 0.0f : panningValue.get()) * 0.02f);
			buffer[bufferIteration] = Sample::load_aligned(lr);
			nextSample();
		}
	}
}
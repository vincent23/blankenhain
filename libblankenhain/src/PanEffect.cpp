#include "PanEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"

PanEffect::PanEffect() : EffectBase(1u)
{
	ParameterBundle* params = getPointerToParameterBundle();
	(params->getParameter(0)) = new FloatParameter(0.f, NormalizedRange(-50.f, 50.f), "pan", "");
}

void PanEffect::process(Sample* buffer, size_t numberOfSamples)
{
	InterpolatedValue<float>& panningValue = getInterpolatedParameter(0);
	for (size_t bufferIteration = 0u; bufferIteration < numberOfSamples; bufferIteration++)
	{
		float panValue = panningValue.get() * .02f;
		float boost = aux::decibelToLinear(3.f);
		float left = aux::max(0.f, -panValue);
		float right = aux::max(0.f, panValue);
		float panLeft = ((1.f - left) + left * boost) * (1.f - right);
		float panRight = ((1.f - right) + right * boost) * (1.f - left);
		buffer[bufferIteration] *= Sample(panLeft, panRight);
		nextSample();
	}
}
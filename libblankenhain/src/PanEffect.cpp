#include "PanEffect.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "AuxFunc.h"

PanEffect::PanEffect() : EffectBase(2u)
{
	ParameterBundle* params = getPointerToParameterBundle();
	(params->getParameter(0)) = new FloatParameter(0.f, NormalizedRange(-50.f, 50.f), "pan", "");
	(params->getParameter(1)) = new FloatParameter(0.f, NormalizedRange(0.f, 1.f), "dummy", "");
}

void PanEffect::process(Sample* buffer, size_t numberOfSamples)
{
	InterpolatedValue<float>& panningValue = getInterpolatedParameter(0);
	for (size_t bufferIteration = 0u; bufferIteration < numberOfSamples; bufferIteration++)
	{
		float panValue = panningValue.get() * .02f;
		aux::performPanning(buffer[bufferIteration], panValue);
		nextSample();
	}
}
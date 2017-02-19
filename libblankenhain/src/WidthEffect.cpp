#include "WidthEffect.h"

#include "ParameterBundle.h"
#include "AuxFunc.h"
#include "InterpolatedValue.h"


// Change the name and define parameters in constructor
WidthEffect::WidthEffect() : EffectBase(1u)
{
	ParameterBundle* params = getPointerToParameterBundle();
	// Insert your stuff here
	(params->getParameter(0)) = new FloatParameter(1.f, NormalizedRange(0.f, 2.f), "width", "");
}

void WidthEffect::process(Sample* buffer, size_t numberOfSamples)
{
	InterpolatedValue<float> width = getInterpolatedParameter(0);
	for (size_t i = 0; i < numberOfSamples; i++)
	{
		Sample in = buffer[i];
		Sample inFlipped = in.flippedChannels();
		Sample widthAsSample = Sample(width.get());
		Sample avg = in * Sample(.5) + inFlipped * Sample(.5);
		buffer[i] = avg * (Sample(1) - widthAsSample) + in * widthAsSample;
		nextSample();
	}
}
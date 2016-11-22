#include "ExampleEffect.h"

#include "ParameterBundle.h"
#include "AuxFunc.h"
#include "InterpolatedValue.h"

#include <algorithm>

// Here, enter the number of GUI parameters you want to have
const unsigned int NUMBER_OF_PARAMETERS = 5u;

// Change the name and define parameters in constructor
ExampleEffect::ExampleEffect() : EffectBase(NUMBER_OF_PARAMETERS), delayLine(size_t(aux::millisecToSamples(2502u)))
{
	ParameterBundle* params = getPointerToParameterBundle();
	// Insert your stuff here
	(params->getParameter(0)) = new FloatParameter(1.f, NormalizedRange(0.01f, 12.f, 1.f), "ratio", "");
	(params->getParameter(1)) = new FloatParameter(10.f, NormalizedRange(0.2f, 15.f, 0.5f), "release", "ms");
	(params->getParameter(2)) = new FloatParameter(0.f, NormalizedRange(-48.f, 0.f, 4.5f), "threshold", "dB");
	(params->getParameter(3)) = new FloatParameter(10.f, NormalizedRange(0.2f, 35.f, 1.f), "attack", "ms");
	(params->getParameter(4)) = new FloatParameter(0.f, NormalizedRange(0.f, 100.f, 1.f), "limiterOn", "");
}

void ExampleEffect::process(Sample* buffer, size_t numberOfSamples)
{
	float ratio = getParameterValue(0).get();
	float release = getParameterValue(1).get();
	float threshold = getParameterValue(2).get();
	float attack = getParameterValue(3).get();
	bool limiterIsOn = getParameterValue(4).get() > 0.5 ? true : false;
	size_t attackTimeInSamples = size_t(aux::millisecToSamples(attack));
	delayLine.setSize(attackTimeInSamples);
	for (size_t i = 0; i < numberOfSamples; i++)
	{
		buffer[i] = delayLine.pushpop(buffer[i]);
	}
	nextSample(numberOfSamples);
}
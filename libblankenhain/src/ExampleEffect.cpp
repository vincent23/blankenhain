#include "ExampleEffect.h"

#include "ParameterBundle.h"
#include "AuxFunc.h"

#include <algorithm>

// Here, enter the number of GUI parameters you want to have
const unsigned int NUMBER_OF_PARAMETERS = 5u;

// Change the name and define parameters in constructor
ExampleEffect::ExampleEffect() : EffectBase(NUMBER_OF_PARAMETERS, 512u), delayLine(size_t(aux::millisecToSamples(2502u)))
{
	ParameterBundle* params = getPointerToParameterBundle();
	// Insert your stuff here
	(params->getParameter(0)) = new ParameterWithProperties(1.f, NormalizedRange(0.01f, 12.f, 1.f), "ratio", "");
	(params->getParameter(1)) = new ParameterWithProperties(10.f, NormalizedRange(0.2f, 15.f, 0.5f), "release", "ms");
	(params->getParameter(2)) = new ParameterWithProperties(0.f, NormalizedRange(-48.f, 0.f, 4.5f), "threshold", "dB");
	(params->getParameter(3)) = new ParameterWithProperties(10.f, NormalizedRange(0.2f, 35.f, 1.f), "attack", "ms");
	(params->getParameter(4)) = new ParameterWithProperties(0.f, NormalizedRange(0.f, 100.f, 1.f), "limiterOn", "");
}

void ExampleEffect::process(Sample* buffer, size_t numberOfSamples, size_t numberOfParameters, float* parameters)
{
	float& ratio = parameters[0];
	float& release = parameters[1];
	float& threshold = parameters[2];
	float& attack = parameters[3];
	bool limiterIsOn = parameters[4] > 0.5 ? true : false;
	size_t attackTimeInSamples = size_t(aux::millisecToSamples(attack));
	delayLine.setSize(attackTimeInSamples);
	for (size_t i = 0; i < numberOfSamples; i++)
	{
		buffer[i] = delayLine.pushpop(buffer[i]);
	}
}
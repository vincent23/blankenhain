//Necessary headers, dont change this
#include "InterpolatedValue.h"
#include "ParameterBundle.h"

// Include the header belonging to this effect
#include "ExampleEffect.h"

// Further includes you might need may be added here:
#include "AuxFunc.h"
//

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

void ExampleEffect::process(Sample* buffer, size_t numberOfSamples, size_t currentTime)
{
	float ratio = getInterpolatedParameter(0).get();
	float release = getInterpolatedParameter(1).get();
	float threshold = getInterpolatedParameter(2).get();
	float attack = getInterpolatedParameter(3).get();
	bool limiterIsOn = getInterpolatedParameter(4).get() == 1.f;
	size_t attackTimeInSamples = size_t(aux::millisecToSamples(attack));
	delayLine.setSize(attackTimeInSamples);
	for (size_t i = 0; i < numberOfSamples; i++)
	{
		buffer[i] = delayLine.pushpop(buffer[i]);
	}

	// You absolutely NEED TO CALL nextSample() in the process function
	// You need to make sure that the appropriate number of interpolation-Iterations
	// are performed by either calling nextSample(numberOfSamples) once or
	// nextSample(1) exactly "numberOfSamples" times.
	nextSample(numberOfSamples);
}
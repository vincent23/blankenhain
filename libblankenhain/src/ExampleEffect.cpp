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
	ParameterBundle& params = getParameterBundle();
	// Insert your stuff here
	params.initParameter(0, new FloatParameter(1.f, NormalizedRange(0.01f, 12.f, 1.f), "ratio", ""));
	params.initParameter(1, new FloatParameter(10.f, NormalizedRange(0.2f, 15.f, 0.5f), "release", "ms"));
	params.initParameter(2, new FloatParameter(0.f, NormalizedRange(-48.f, 0.f, 4.5f), "threshold", "dB"));
	params.initParameter(3, new FloatParameter(10.f, NormalizedRange(0.2f, 35.f, 1.f), "attack", "ms"));
	params.initParameter(4, new FloatParameter(0.f, NormalizedRange(0.f, 100.f, 1.f), "limiterOn", ""));
}

void ExampleEffect::process(Sample* buffer, size_t numberOfSamples, size_t currentTime)
{
	float ratio = interpolatedParameters.get(0);
	float release = interpolatedParameters.get(1);
	float threshold = interpolatedParameters.get(2);
	float attack = interpolatedParameters.get(3);
	bool limiterIsOn = interpolatedParameters.get(4) == 1.f;
	size_t attackTimeInSamples = size_t(aux::millisecToSamples(attack));
	delayLine.setSize(attackTimeInSamples);
	for (size_t i = 0; i < numberOfSamples; i++)
	{
		buffer[i] = delayLine.pushpop(buffer[i]);
	}
}
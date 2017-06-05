#include "InversePhaseEffect.h"

// Change the name and define parameters in constructor
InversePhaseEffect::InversePhaseEffect() : EffectBase(0u, false)
{

}

void InversePhaseEffect::process(Sample* buffer, size_t numberOfSamples, size_t currentTime)
{
	for (size_t i = 0; i < numberOfSamples; i++)
	{
		Sample& in = buffer[i];
		in *= Sample(-1.);
	}
}

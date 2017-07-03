#include "EffectBase.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "FpuState.h"
#include "Options.h"

EffectBase::EffectBase(unsigned int numberOfParameters, bool useTempoData)
	: tempodata(useTempoData)
	, paramBundle(numberOfParameters)
	, interpolatedParameters(numberOfParameters)
	, nextModulation(numberOfParameters == 0 ? nullptr : new float[numberOfParameters])
	, fpuState()
{ 

}

EffectBase::~EffectBase()
{
	if (nextModulation != nullptr) delete[] nextModulation;
}

const bool EffectBase::effectUsesTempoData() const
{
	return tempodata.usesTempoData;
}

void EffectBase::setTempoData(float bpm, unsigned int position)
{
	tempodata.position = position;
	tempodata.bpm = bpm;
};

void EffectBase::incrementTempoDataPosition(unsigned int increment)
{
	this->tempodata.position += increment;
}

const unsigned int EffectBase::getDelay() const
{
  return this->delayEffectProducesInSamples;
}

TempoData const& EffectBase::getTempoData() const
{
	return tempodata;
}


void EffectBase::processBlock(Sample* buffer, size_t numberOfSamples)
{
	FpuState fpuState;

	interpolatedParameters.setSteps(numberOfSamples);

	// TODO find a better way to do initalization
	if (!initializedParameters) 
	{
		for (unsigned int parameterIndex = 0u; parameterIndex < getNumberOfParameters(); parameterIndex++) 
		{
			float value = paramBundle.getParameter(parameterIndex)->getValueUnnormalized();
			// this emulates the previous block end, which we set to the unmodulated current parameter value
			interpolatedParameters.setCurrent(parameterIndex, value);
		}
		initializedParameters = true;
	}

	// calculate modulation at the next block start using current (modulated) parameter values
	// this means the modulation of a parameter controlling some other modulation is delayed by one block length
	for (unsigned int parameterIndex = 0; parameterIndex < getNumberOfParameters(); parameterIndex++) {
		nextModulation[parameterIndex] = 0;
	}
	getModulation(nextModulation, numberOfSamples);

	for (unsigned int parameterIndex = 0; parameterIndex < getNumberOfParameters(); parameterIndex++) 
	{
		FloatParameter* parameter = paramBundle.getParameter(parameterIndex);
		
		if (!parameter->canBeModulated())
		{
			// No or modulation for Bool / Discrete / Option Param
			float value = parameter->getValueUnnormalized();
			interpolatedParameters.setCurrent(parameterIndex, value);
		}
		else
		{
			// FloatParameter are modulated

			// get normalized value at start of next block
			parameter->next(numberOfSamples);
			float normalizedNextValue = parameter->getValueNormalized();

			// we clamp the normalized value after modulation
			float normalizedNextValueModulated = normalizedNextValue + nextModulation[parameterIndex];
			if (normalizedNextValueModulated < 0.f)
				normalizedNextValueModulated = 0.f;
			else if (normalizedNextValueModulated > 1.f)
				normalizedNextValueModulated = 1.f;

			float nextValue = parameter->range.fromNormalized(normalizedNextValueModulated);
			interpolatedParameters.setTarget(parameterIndex, nextValue);
		}
	}

	process(buffer, numberOfSamples, timeInSamples);
	timeInSamples += numberOfSamples;

#ifdef _LIBBLANKENHAIN_ENABLE_NANCHECK
	for (unsigned int i = 0u; i < numberOfSamples; ++i)
	{
		if (buffer[i].avgValue() != buffer[i].avgValue())
			throw std::runtime_error("nan detected");
	}
#endif
	interpolatedParameters.next(numberOfSamples);
}

ParameterBundle& EffectBase::getParameterBundle()
{
	return (this->paramBundle);
}

unsigned int EffectBase::getNumberOfParameters() const
{
	return this->paramBundle.getNumberOfParameters();
}

// This function is empty as not all effects have modulation
// (virtual void would be overkill)
void EffectBase::getModulation(float* modulationValues, size_t sampleOffset)
{ }

unsigned int const& EffectBase::getCurrentTime() const
{
	return this->timeInSamples;
}

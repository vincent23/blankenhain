#include "EffectBase.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"
#include "FpuState.h"
#include "Options.h"

EffectBase::EffectBase(unsigned int numberOfParameters, bool useTempoData)
	: tempodata(useTempoData)
	, paramBundle(numberOfParameters == 0 ? nullptr : new ParameterBundle(numberOfParameters))
	, parameterValues(numberOfParameters == 0 ? nullptr : new InterpolatedValue<float>[numberOfParameters])
	, nextModulation(numberOfParameters == 0 ? nullptr : new float[numberOfParameters])
{ 

}

EffectBase::~EffectBase()
{
	if (paramBundle != nullptr)
	{
		for (size_t i = 0u; i < paramBundle->getNumberOfParameters(); i++)
		{
			if (paramBundle->getParameter(i) != nullptr)
				delete paramBundle->getParameter(i);

			paramBundle->getParameter(i) = nullptr;
		}
		delete paramBundle;
	}

	if (parameterValues != nullptr) delete[] parameterValues;

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

#ifdef  _LIBBLANKENHAIN_ENABLE_FPU_ROUNDING_CHECK
	unsigned short bar = 0u;
	_asm FSTCW bar
	if (bar != 3711)
	{
		throw "fpu rounding flag wrong";
		puts("fpu rounding flag wrong");
	}
#endif
	unsigned short fcw = 3711;
	__asm fldcw fcw;
	FpuState fpuState;


	// TODO find a better way to do initalization
	if (!initializedParameters) 
	{
		for (unsigned int parameterIndex = 0; parameterIndex < getNumberOfParameters(); parameterIndex++) {
			float value = paramBundle->getParameter(parameterIndex)->getValueUnnormalized();
			// this emulates the previous block end, which we set to the unmodulated current parameter value
			parameterValues[parameterIndex] = InterpolatedValue<float>(value, value, 1);
		}
		initializedParameters = true;
	}

	// calculate modulation at the next block start using current (modulated) parameter values
	// this means the modulation of a parameter controlling some other modulation is delayed by one block length
	for (unsigned int parameterIndex = 0; parameterIndex < getNumberOfParameters(); parameterIndex++) {
		nextModulation[parameterIndex] = 0;
	}
	getModulation(nextModulation, numberOfSamples);

	for (unsigned int parameterIndex = 0; parameterIndex < getNumberOfParameters(); parameterIndex++) {
		FloatParameter* parameter = paramBundle->getParameter(parameterIndex);
		
		if (!parameter->canBeModulated())
		{
			// No or modulation for Bool / Discrete / Option Param
			parameterValues[parameterIndex] = InterpolatedValue<float>(parameter->getValueUnnormalized());
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


			float previousValue = parameterValues[parameterIndex].get();
			float nextValue = parameter->fromNormalized(normalizedNextValueModulated);
			parameterValues[parameterIndex] = InterpolatedValue<float>(previousValue, nextValue, numberOfSamples);
		}
	}
	process(buffer, numberOfSamples, timeInSamples);
	timeInSamples += numberOfSamples;



	for (unsigned int i = 0u; i < numberOfSamples; ++i)
	{
#ifdef _LIBBLANKENHAIN_ENABLE_NANCHECK
		if (buffer[i].avgValue() != buffer[i].avgValue())
			throw "nan detected";
#endif
	}

#ifdef  _LIBBLANKENHAIN_ENABLE_FPU_ROUNDING_CHECK
	bar = 0u;
	_asm FSTCW bar
	if (bar != 3711)
	{
		throw "fpu rounding flag wrong";
		puts("fpu rounding flag wrong");
	}
#endif

}

ParameterBundle* EffectBase::getPointerToParameterBundle() const
{
	return (this->paramBundle);
}

const unsigned int EffectBase::getNumberOfParameters() const
{
	if (this->paramBundle == nullptr)
		return 0u;
	else
		return this->paramBundle->getNumberOfParameters();
}

// This function is empty as not all effects have modulation
// (virtual void would be overkill)
void EffectBase::getModulation(float* modulationValues, size_t sampleOffset)
{ }

/**
 * Get interpolated parameter containing unnormalized values.
 */
InterpolatedValue<float>& EffectBase::getInterpolatedParameter(unsigned int parameterIndex) const
{
	// parameterValues contain results of parameter->getValueUnnormalized()
	return parameterValues[parameterIndex];
}

void EffectBase::nextSample(unsigned int steps) const
{
	for (unsigned int parameterIndex = 0; parameterIndex < getNumberOfParameters(); parameterIndex++) {
		getInterpolatedParameter(parameterIndex).next(steps);
	}
}

void EffectBase::nextSample() const
{
	for (unsigned int parameterIndex = 0; parameterIndex < getNumberOfParameters(); parameterIndex++) {
		getInterpolatedParameter(parameterIndex).next();
	}
}

unsigned int const& EffectBase::getCurrentTime() const
{
	return this->timeInSamples;
}

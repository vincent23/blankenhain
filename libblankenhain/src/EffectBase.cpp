#include "EffectBase.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"

#include <utility>

EffectBase::EffectBase(unsigned int numberOfParameters)
	: params(new ParameterBundle(numberOfParameters))
	, parameterValues(new InterpolatedValue[numberOfParameters])
	, nextModulation(new float[numberOfParameters])
{ }

EffectBase::~EffectBase()
{
	for (size_t i = 0u; i < params->getNumberOfParameters(); i++) {
		if (params->getParameter(i) != nullptr) delete params->getParameter(i);
		params->getParameter(i) = nullptr;
	}
	if (params != nullptr) delete params;
	params = nullptr;
	if (parameterValues != nullptr) delete[] parameterValues;
	parameterValues = nullptr;
	if (nextModulation != nullptr) delete[] nextModulation;
	nextModulation = nullptr;
}

void EffectBase::processBlock(Sample* buffer, size_t numberOfSamples)
{
	// TODO find a better way to do initalization
	if (!initializedParameters) {
		for (unsigned int parameterIndex = 0; parameterIndex < getNumberOfParameters(); parameterIndex++) {
			float value = params->getParameter(parameterIndex)->getValueUnnormalized();
			// this emulates the previous block end, which we set to the unmodulated current parameter value
			parameterValues[parameterIndex] = InterpolatedValue(value, value, 1);
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
		FloatParameter* parameter = params->getParameter(parameterIndex);

		// get normalized value at start of next block
		parameter->next(numberOfSamples);
		float normalizedNextValue = parameter->getValueNormalized();

		// TODO probably we should clamp the normalized value somewhere
		float normalizedNextValueModulated = normalizedNextValue + nextModulation[parameterIndex];
		float previousValue = parameterValues[parameterIndex].get();
		float nextValue = parameter->fromNormalized(normalizedNextValueModulated);
		parameterValues[parameterIndex] = InterpolatedValue(previousValue, nextValue, numberOfSamples);
	}

	process(buffer, numberOfSamples);
}

ParameterBundle* EffectBase::getPointerToParameterBundle() const
{
	return (this->params);
}

unsigned int EffectBase::getNumberOfParameters() const
{
	return this->params->getNumberOfParameters();
}

void EffectBase::getModulation(float* modulationValues, size_t sampleOffset)
{ }

InterpolatedValue& EffectBase::getParameterValue(unsigned int parameterIndex) const
{
	return parameterValues[parameterIndex];
}

void EffectBase::nextSample(unsigned int steps) const
{
	for (unsigned int parameterIndex = 0; parameterIndex < getNumberOfParameters(); parameterIndex++) {
		getParameterValue(parameterIndex).next(steps);
	}
}

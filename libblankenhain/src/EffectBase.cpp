#include "EffectBase.h"

#include "ParameterBundle.h"
#include "InterpolatedValue.h"

#include <utility>

EffectBase::EffectBase(unsigned int numberOfParameters, bool useTempoData)
	: tempodata(useTempoData)
	, paramBundle(new ParameterBundle(numberOfParameters))
	, parameterValues(new InterpolatedValue<float>[numberOfParameters])
	, nextModulation(new float[numberOfParameters])
{ }

EffectBase::~EffectBase()
{
	for (size_t i = 0u; i < paramBundle->getNumberOfParameters(); i++) {
		if (paramBundle->getParameter(i) != nullptr) delete paramBundle->getParameter(i);
		paramBundle->getParameter(i) = nullptr;
	}
	if (paramBundle != nullptr) delete paramBundle;
	paramBundle = nullptr;
	if (parameterValues != nullptr) delete[] parameterValues;
	parameterValues = nullptr;
	if (nextModulation != nullptr) delete[] nextModulation;
	nextModulation = nullptr;
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

void EffectBase::processBlock(Sample* buffer, size_t numberOfSamples)
{
	// TODO find a better way to do initalization
	if (!initializedParameters) {
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

		// get normalized value at start of next block
		parameter->next(numberOfSamples);
		float normalizedNextValue = parameter->getValueNormalized();

		// we clamp the normalized value after modulation
		float normalizedNextValueModulated = normalizedNextValue + nextModulation[parameterIndex];
		if (normalizedNextValueModulated < 0.f)
			normalizedNextValueModulated = 0.f;
		else if(normalizedNextValueModulated > 1.f)
			normalizedNextValueModulated = 1.f;


		float previousValue = parameterValues[parameterIndex].get();
		float nextValue = parameter->fromNormalized(normalizedNextValueModulated);
		parameterValues[parameterIndex] = InterpolatedValue<float>(previousValue, nextValue, numberOfSamples);
	}
	process(buffer, numberOfSamples);

	if (this->effectUsesTempoData())
		this->tempodata.position += numberOfSamples;

}

ParameterBundle* EffectBase::getPointerToParameterBundle() const
{
	return (this->paramBundle);
}

const unsigned int EffectBase::getNumberOfParameters() const
{
	return this->paramBundle->getNumberOfParameters();
}

// This function is empty as not all effects have modulation
// (virtual void would be overkill)
void EffectBase::getModulation(float* modulationValues, size_t sampleOffset)
{ }

InterpolatedValue<float>& EffectBase::getInterpolatedParameter(unsigned int parameterIndex) const
{
	return parameterValues[parameterIndex];
}

void EffectBase::nextSample(unsigned int steps) const
{
	for (unsigned int parameterIndex = 0; parameterIndex < getNumberOfParameters(); parameterIndex++) {
		getInterpolatedParameter(parameterIndex).next(steps);
	}
}

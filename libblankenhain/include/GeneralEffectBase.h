#pragma once

#include "Sample.h"

class ParameterBundle;
class InterpolatedValue;

class GeneralEffectBase : public AlignedType
{
public:
	GeneralEffectBase(unsigned int numberOfParameters);
	virtual ~GeneralEffectBase();

	void processBlock(Sample* buffer, size_t numberOfSamples);
	ParameterBundle* getPointerToParameterBundle() const;
	unsigned int getNumberOfParameters() const;

	/**
	* Applies the effect to @a numberOfSamples audio samples in @a buffer.
	* This method must also call nextSample() exactly @a numberOfSamples times.
	* @param[in,out] buffer The input buffer which is modified by the effect.
	* @param[in] numberOfSamples The number of samples in @a buffer.
	*/
	virtual void process(Sample* buffer, size_t numberOfSamples) = 0;

	/**
	* Computes modulating values using current parameters.
	* These values will be added to the normalized representation of the parameters for modulation.
	* @param[out] modulationValues An array of modulation values of length @a getNumberOfParameters()
	* @param[in] sampleOffset Relative sample position at which modulation is evaluated
	*/
	virtual void getModulation(float* modulationValues, size_t sampleOffset = 0);

protected:
	InterpolatedValue& getParameterValue(unsigned int parameterIndex) const;
	void nextSample(unsigned int steps = 1) const;

private:
	ParameterBundle* params;
	InterpolatedValue* parameterValues;
	float* nextModulation;
	bool initializedParameters = false;
};
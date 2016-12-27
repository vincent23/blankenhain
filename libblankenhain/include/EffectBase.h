#pragma once

#include "Sample.h"
#include "InterpolatedValue.h"

class ParameterBundle;

class EffectBase : public AlignedType
{
public:
	EffectBase(unsigned int numberOfParameters);
	virtual ~EffectBase();

	/**  This function will call the process() function.
	 * Processing in bh2 is performed blockwise. Here we prepare the parameterValues array
	 * which is to be used during the process() function of each effect. Do not access paramBundle.
	 *
	 * In short, this passes on the call to the provided virtual void process function() in a manner so that everyhting is ok :-)
	 **/
	void processBlock(Sample* buffer, size_t numberOfSamples);

	/// Helper functions
	ParameterBundle* getPointerToParameterBundle() const;
	unsigned int getNumberOfParameters() const;


	/**
	* Computes modulating values using current parameters.
	* These values will be added to the normalized representation of the parameters for modulation.
	* @param[out] modulationValues An array of modulation values of length @a getNumberOfParameters()
	* @param[in] sampleOffset Relative sample position at which modulation is evaluated
	*/
	virtual void getModulation(float* modulationValues, size_t sampleOffset = 0);

protected:
	InterpolatedValue<float>& getInterpolatedParameter(unsigned int parameterIndex) const;

	/** 
	 * Adjusts parameter interpolation by moving ALL of their interpolation foreward
	 * Call as shorthand when your synth doesnt need current values of the parameters but only approximate ones.
	 **/
	void nextSample(unsigned int steps = 1) const;

	/**
	* Applies the effect to @a numberOfSamples audio samples in @a buffer.
	* This method must also call nextSample() exactly @a numberOfSamples times.
	* @param[in,out] buffer The input buffer which is modified by the effect.
	* @param[in] numberOfSamples The number of samples in @a buffer.
	*/
	virtual void process(Sample* buffer, size_t numberOfSamples) = 0;

private:
	ParameterBundle* paramBundle;
	InterpolatedValue<float>* parameterValues;
	float* nextModulation;
	bool initializedParameters = false;
};
#pragma once

#include "Sample.h"
#include "InterpolatedValue.h"
#include "AlignedType.h"

class ParameterBundle;

/**
 * Container for bpm and position. Accquired at the beginning of every processReplacing funtion.
 * Updated during processReplacing.
 */
class TempoData
{
public:
	TempoData(bool uses) : usesTempoData(uses), bpm(0.f), position(0u) {};
	const bool usesTempoData;
	float bpm;
	unsigned int position;
};

class EffectBase : public AlignedType
{
public:
	EffectBase(unsigned int numberOfParameters, bool usesTempoData = false);
	virtual ~EffectBase();

	/**  This function will call the process() function.
	 * Processing in bh2 is performed blockwise. Here we prepare the parameterValues array
	 * which is to be used during the process() function of each effect. Do not access paramBundle.
	 *
	 * ProcessBlock also accounts for updating the timeData if the effect uses BPM and position information from the host
	 * after processing each block.
	 *
	 * In short, this passes on the call to the provided virtual void process function() in a manner so that everyhting is ok :-)
	 **/
	void  processBlock(Sample* buffer, size_t numberOfSamples);

	/// Helper functions
	ParameterBundle* getPointerToParameterBundle() const;
	const unsigned int getNumberOfParameters() const;


	/**
	 * Computes modulating values using current parameters.
	 * These values will be added to the normalized representation of the parameters for modulation.
	 * @param[out] modulationValues An array of modulation values of length @a getNumberOfParameters()
	 * @param[in] sampleOffset Relative sample position at which modulation is evaluated.
	 *
	 * You may use tempoData in this function if effect uses it (add sample offset to tempodata.postion to get the very
	 * current position).
	 */
	virtual void getModulation(float* modulationValues, size_t sampleOffset = 1);

	/**
	 * Returns if effect is synced to host and uses BPM
	 */
	const bool effectUsesTempoData() const;

	/**
	 * Called by PluginBase (and therein by host) to update BPM and position
	 */
	void setTempoData(float bpm, unsigned int position);

	void incrementTempoDataPosition(unsigned int increment);

	unsigned int const& getCurrentTime() const;

  const unsigned int getDelay() const;

protected:
	/**
	 * Use this, and only this function, to get access to current parameter values
	 */
	InterpolatedValue<float>& getInterpolatedParameter(unsigned int parameterIndex) const;

	/** 
	 * Adjusts parameter interpolation by moving ALL of their interpolation foreward
	 * Call as shorthand when your synth doesnt need current values of the parameters but only approximate ones.
	 *
	 * See also: FloatParameter::next(uint). This may be called manually instead of EffectBase::nextSample()
	 **/
	void nextSample(unsigned int steps) const;
	void nextSample() const;

	/**
	* Applies the effect to @a numberOfSamples audio samples in @a buffer.
	* This method must also call nextSample() exactly @a numberOfSamples times.
	* @param[in,out] buffer The input buffer which is modified by the effect.
	* @param[in] numberOfSamples The number of samples in @a buffer.
	* @param[in] currentGlobalTime The blankenhain time when the process function is called
	*/
	virtual void process(Sample* buffer, size_t numberOfSamples, size_t currentTime) = 0;

	/**
	 * Holds information about sync with host DAW, BPM and position
	 */
	TempoData tempodata;

  unsigned int delayEffectProducesInSamples = 0;

private:	
	unsigned int timeInSamples = 0;
	ParameterBundle* paramBundle;
	InterpolatedValue<float>* parameterValues;
	float* nextModulation;
	bool initializedParameters = false;

};

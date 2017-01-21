#pragma once

#include <string>
#include <vector>
#include <atomic>
#include "FloatParameter.h"
#pragma once
#include "ParameterBundle.h"

/**
 * Class that handles parameters for the VST plugin.
 * In libblankenhain Parameters are handled as @see FloatParameter
 * wrapped in a @see ParameterBundle.
 *
 * As VST plugin calls are very random (in regards to when they are called by the host)
 * we build a "Chinese Wall" between parameter values the host can access and values
 * the effect code actually uses for processing.
 *
 * We have an array of atomic float values in this class. If a user changes a parameter
 * in the host DAW, this is the first place they will be updated (hence atomic).
 * Every so often, the updateParameters() function is called to write the current parameters
 * stored in the float vector to the ParameterBundle. Once the ParameterBundle is updated,
 * those values will be used for actual processing.
 */
class PluginParameterBundle
{
public:
	PluginParameterBundle(ParameterBundle* params);

	void setPluginParameter(unsigned int const& in, float const& normalizedValue);
	/**
	 * Updates Parameters by setting their target value to the value found in the host DAW
	 * Interpolation is performed
	 */
	void updateParameters();
	std::string getParameterName(unsigned int const& in) const;
	float getParameterNormalized(unsigned int const& index) const;
	float getParameterUnnormalized(unsigned int const& index) const;
	std::string getParameterUnit(unsigned int const& index) const;
	/*
	 * Called only by Host via setParameter or setParameterAutomated
	 */
	// You can access stuff via get.
	// All access is const-&. Don't break encapsulation
	const FloatParameter* getParameter(unsigned int const& index) const;
	// This used to exist, but it broke the encapsulation design. Only
	// the call to "PluginParameterBundle.updateParameters()" may
	// directly change values in the (bh) parameterBundle
	//const ParameterBundle& getParameterBundle() const;
	unsigned int getNumberOfParameters() const;

private:
	std::vector<std::atomic<float>> currentParametersFromHost;

	ParameterBundle* parameterBundle;
};